#include <stdafx.h>
#include "ViewerWindow.h"

const wchar_t* CViewerWindow::ClassName = L"CViewerWindow";
const wchar_t* CViewerWindow::ViewerApplicationName = L"Powerspace Viewer";
const UINT TICK_LENGTH = 1000;

CViewerWindow::CViewerWindow( CStage& _stage, CViewport& _viewport, CCanvas& _canvas, CScriptHolder& _holder ) :
	windowHeight( 600 ), windowWidth( 800 ), viewport( _viewport ), canvas( _canvas ), holder(_holder),
	handle( nullptr ), stage( _stage ), scriptEngine( stage ), activeId( CObjectIdGenerator::GetEmptyId() ), colorBuf( -1 ),
	viewerIsRunning( true )
{}

CViewerWindow::~CViewerWindow()
{}

//
//  FUNCTION: RegisterClass()
//
//  PURPOSE: Registers the viewer window class.
//
bool CViewerWindow::RegisterClass()
{
	HBRUSH backgroundBrush = ::CreateHatchBrush( HS_CROSS, 0x66FF66 );
	HMODULE instance = ::GetModuleHandleW( nullptr );

	WNDCLASSEX windowClassInformation;
	windowClassInformation.cbSize = sizeof( WNDCLASSEX );
	windowClassInformation.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassInformation.lpfnWndProc = WindowProc;
	windowClassInformation.cbClsExtra = 0;
	windowClassInformation.cbWndExtra = 2 * sizeof( LONG_PTR );
	windowClassInformation.hInstance = instance;
	windowClassInformation.hIcon = ::LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );
	windowClassInformation.hCursor = ::LoadCursor( nullptr, IDC_ARROW );
	windowClassInformation.hbrBackground = backgroundBrush;
	windowClassInformation.lpszMenuName = MAKEINTRESOURCE( IDR_MENU2 );
	windowClassInformation.lpszClassName = ClassName;
	windowClassInformation.hIconSm = ::LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );

	return (::RegisterClassEx( &windowClassInformation ) != 0);
}

//
//  FUNCTION: Create()
//
//  PURPOSE: Creates the viewer window
//
bool CViewerWindow::Create()
{
	HINSTANCE hInstance = ::GetModuleHandle( nullptr );

	handle = ::CreateWindowEx( 0, ClassName, ViewerApplicationName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowHeight, windowWidth, nullptr, nullptr,
		hInstance, this );
	if( handle != 0 ) {
		enableTimer( TICK_LENGTH );
	}
	return (handle != 0);
}

void CViewerWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CViewerWindow::GetHandle() const
{
	return handle;
}

//
//  FUNCTION: WindowProc()
//
//  PURPOSE: Main window proc
//
LRESULT CViewerWindow::WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_NCCREATE ) {
		::SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );

		return ::DefWindowProc( handle, msg, wParam, lParam );
	}

	CViewerWindow* wndPtr = reinterpret_cast<CViewerWindow*>(::GetWindowLongPtr( handle, 0 ));

	switch( msg ) {
	case WM_CREATE:
		wndPtr->onCreate();
		return 0;

	case WM_CLOSE:
		wndPtr->onClose();
		return 0;

	case WM_PAINT:
		wndPtr->onPaint();
		return 0;

	case WM_SIZE:
		wndPtr->onSize();
		return 0;

	case WM_MOUSEMOVE:
		wndPtr->onMouseMove( wParam, lParam );
		return 0;

	case WM_LBUTTONDOWN:
		wndPtr->onMouseClick( msg, wParam, lParam );
		return 0;

	case WM_LBUTTONDBLCLK:
		wndPtr->onMouseClick( msg, wParam, lParam );
		return 0;

	case WM_COMMAND:
		wndPtr->onCommand( wParam, lParam );
		return 0;
	case WM_TIMER:
	{
		wndPtr->onTimer();
		return 0;
	}

	default:
		return ::DefWindowProc( handle, msg, wParam, lParam );
	}
}

void CViewerWindow::onCreate() {}

void CViewerWindow::onClose()
{
	if( ::MessageBox( handle, L"Really quit?", ViewerApplicationName, MB_OKCANCEL ) == IDOK ) {
		::DestroyWindow( handle );
	}
}

//
//  FUNCTION: onTimer()
//
//  PURPOSE: HERE WE HANDLE THE ONTICK EVENTS
//
void CViewerWindow::onTimer()
{
	if( !viewerIsRunning )
		return;

	for( auto pair : stage.GetObjects() ) {

		auto scripts = pair.second->GetScripts();
        auto names = std::vector<IdType>();
        for( auto script : scripts ) {
            names.push_back( script.GetName() );
        }
		updateColorWithBuffer( activeId, ColorBufferActionType::RestoreColor );
		if( !scripts.empty() ) {
			MessageBoxW( NULL, activeId.c_str(), L"Viewer", NULL );
			scriptEngine.RunScripts( activeId, EventType::EventTick, names );
		}
		updateColorWithBuffer( activeId, ColorBufferActionType::SetColor );
	}

	RECT rect;
	::GetClientRect( handle, &rect );
	::InvalidateRect( handle, &rect, false );
}

//
//  FUNCTION: onPaint()
//
//  PURPOSE: repainter
//
void CViewerWindow::onPaint()
{
	PAINTSTRUCT paintStruct;
	HDC hdc = ::BeginPaint( handle, &paintStruct );
	RECT rect;
	::GetClientRect( handle, &rect );

	int winWidth = rect.right - rect.left;
	int winHeight = rect.bottom + rect.left;

	HDC Memhdc = ::CreateCompatibleDC( hdc );
	HBITMAP Membitmap = ::CreateCompatibleBitmap( hdc, winWidth, winHeight );
	::SelectObject( Memhdc, Membitmap );

	HBRUSH canvasBrush = ::CreateHatchBrush( HS_CROSS, BLUE_FOR_CANVAS_CROSS );

	::FillRect( Memhdc, &paintStruct.rcPaint, canvasBrush );

	stage.ClipAndDrawObjects( Memhdc );

	BOOL result = ::BitBlt( hdc, 0, 0, winWidth, winHeight, Memhdc, 0, 0, SRCCOPY );
	if( !result ) {
		::MessageBox( handle, L"BitBlt : onPaint() : CViewerWindow", L"Error", MB_ICONERROR );
		::PostQuitMessage( NULL );
	}

	::DeleteObject( Membitmap );
	::DeleteDC( Memhdc );
	::EndPaint( handle, &paintStruct );
}

//
//  FUNCTION: onSize()
//
//  PURPOSE: resize handler
//
void CViewerWindow::onSize()
{
	RECT area;
	::GetClientRect( handle, &area );
	HDC tmp = ::GetDC( handle );
	bitmapContext = ::CreateCompatibleDC( tmp );

	bitmapWidth = area.right - area.left + 1;
	bitmapHeight = area.bottom - area.top + 1;

	bitmap = ::CreateCompatibleBitmap( tmp, bitmapWidth, bitmapHeight );
	::SelectObject( bitmapContext, bitmap );

	if( ::SetGraphicsMode( bitmapContext, GM_ADVANCED ) == 0 ) {
		::MessageBox( handle, L"Error", L"Error", MB_ICONERROR | MB_OK );
	}

	::ReleaseDC( handle, tmp );
	::DeleteDC( tmp );
}

void CViewerWindow::onMouseMove( const WPARAM wParam, const LPARAM lParam )
{
	// TODO: handle mouse moving
	// E.g. mouse is on the object => change the color

	UNREFERENCED_PARAMETER( wParam );

	POINT mouseCoords = getMouseCoords( lParam );

	IdType topId = CObjectIdGenerator::GetEmptyId();

	for( auto pair : stage.GetObjects() ) {
		TBox curBox = pair.second->GetContainingBox();

		if( isPointInBox( curBox, mouseCoords ) ) {
			topId = pair.second->GetId();
		}
	}

	if( topId == CObjectIdGenerator::GetEmptyId() )
		return;

	/*
	in this case we can somehow mark the object, but it looks terrible

	IDrawablePtr topObj = stage.GetObjectById( topId );

	COLORREF color = topObj->GetColor();
	topObj->SetColor( RGB( 0, 0, 0 ) );

	RECT rect;
	GetClientRect( handle, &rect );
	InvalidateRect( handle, &rect, false );

	onPaint();

	topObj->SetColor( color );

	GetClientRect( handle, &rect );
	InvalidateRect( handle, &rect, false );
	*/
}

//
//  FUNCTION: onMouseClick()
//
//  PURPOSE: HERE WE HANDLE THE ONCLICK EVENTS
//
void CViewerWindow::onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	if( !viewerIsRunning )
		return;

	UNREFERENCED_PARAMETER( wParam );
	UNREFERENCED_PARAMETER( msg );

	POINT mouseCoords = getMouseCoords( lParam );

	// storage space for usual color of currently active object
	IdType prevActiveId = activeId;
	activeId = CObjectIdGenerator::GetEmptyId();


	for( auto pair : stage.GetObjects() ) {
		TBox curBox = pair.second->GetContainingBox();

		if( isPointInBox( curBox, mouseCoords ) ) {
			activeId = pair.second->GetId();
		}
	}

	// same active object as before => no action needed
	// // Strange behaviour. May be I want to change cycled colors,
	// // this way, I have to change focus every time. 
	// // Following code will be commented until the discussion.
	// if( activeId == prevActiveId )
	//	 return;



	// clicked on new object => have to process it
	updateColorWithBuffer( prevActiveId, ColorBufferActionType::RestoreColor );
	if( activeId != CObjectIdGenerator::GetEmptyId() ) {


		stage.GetObjectById( activeId )->SetColor( static_cast<COLORREF> (colorBuf * 0.8) );

		auto scripts = stage.GetObjectById( activeId )->GetScripts();
        auto names = std::vector<IdType>();
        for( auto script : scripts ) {
            names.push_back( script.GetName() );
        }
		if( !scripts.empty() ) {
			scriptEngine.RunScripts( activeId, EventType::EventClick, names);
		}
	}
	updateColorWithBuffer( prevActiveId, ColorBufferActionType::SetColor );

	RECT rect;
	::GetClientRect( handle, &rect );
	::InvalidateRect( handle, &rect, false );
}

void CViewerWindow::onCommandMenu( WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );

	switch LOWORD( wParam )
	{
	case ID_CONTROL_PLAY:
		viewerIsRunning = !viewerIsRunning;
		if( viewerIsRunning ) {
			enableTimer( TICK_LENGTH );
		} else {
			disableTimer();
		}

		HMENU pMenu = ::GetMenu( handle );

		if( pMenu != NULL ) {
			if( viewerIsRunning )
				::CheckMenuItem( pMenu, ID_CONTROL_PLAY, MF_UNCHECKED | MF_BYCOMMAND );
			else
				::CheckMenuItem( pMenu, ID_CONTROL_PLAY, MF_CHECKED | MF_BYCOMMAND );
		} else {
			::MessageBox( handle, L"Menu not found", L"Error", MB_ICONERROR );
			::PostQuitMessage( NULL );
		}

		::SetMenu( handle, pMenu );
		break;
	}
}

void CViewerWindow::onCommand( WPARAM wParam, LPARAM lParam )
{
	switch( HIWORD( wParam ) ) {
	case 0:
		onCommandMenu( wParam, lParam );
		break;
	}
}

POINT CViewerWindow::getMouseCoords( LPARAM lParam )
{
	POINT mouseCoords;

	mouseCoords.x = LOWORD( lParam );
	mouseCoords.y = HIWORD( lParam );

	return mouseCoords;
}

bool CViewerWindow::isPointInBox( TBox box, POINT point )
{
	if( point.x <= box.right &&
		point.x >= box.left &&
		point.y <= box.bottom &&
		point.y >= box.top )
		return true;
	else
		return false;
}

void CViewerWindow::enableTimer( int timeDelay, int timerId )
{
	::SetTimer( handle, timerId, timeDelay, 0 );
}

void CViewerWindow::disableTimer( int timerId )
{
	::KillTimer( handle, timerId );
}

void CViewerWindow::updateColorWithBuffer( IdType prevActiveId, ColorBufferActionType actionType )
{
	switch( actionType ) {
	case ColorBufferActionType::RestoreColor:
	{
		if( prevActiveId != CObjectIdGenerator::GetEmptyId() ) {
			stage.GetObjectById( prevActiveId )->SetColor( colorBuf );
		}
		break;
	}
	case ColorBufferActionType::SetColor:
	{
		if( activeId != CObjectIdGenerator::GetEmptyId() ) {
			colorBuf = stage.GetObjectById( activeId )->GetColor();
			stage.GetObjectById( activeId )->SetColor( static_cast<COLORREF> (colorBuf * 0.9) );
		}
		break;
	}
	}
}