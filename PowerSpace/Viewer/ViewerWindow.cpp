#include <stdafx.h>
#include "resource.h"
#include "Resource.h"
#include "ViewerWindow.h"

const wchar_t* CViewerWindow::ClassName = L"CViewerWindow";
const wchar_t* CViewerWindow::ViewerApplicationName = L"Powerspace Viewer";
const UINT TICK_LENGTH = 1000;
const double SPEED_MULTIPLIER = 1.1;
UINT SPEED = TICK_LENGTH;

CViewerWindow::CViewerWindow( CStage& _stage, CViewport& _viewport, CCanvas& _canvas ) :
	windowHeight( 1376 ), windowWidth( 768 ), viewport( _viewport ), canvas( _canvas ),
	handle( nullptr ), stage( _stage ), scriptEngine( stage ), activeId( 0 ), colorBuf( -1 ),
	viewerIsRunning( true ), currentMovingState( MSV_None )
{
	canvasPoint = _viewport.GetZeroLocation();
}

CViewerWindow::~CViewerWindow()
{}

//
//  FUNCTION: RegisterClass()
//
//  PURPOSE: Registers the viewer window class.
//
bool CViewerWindow::RegisterClass()
{
	HBRUSH backgroundBrush = ::CreateHatchBrush( HS_CROSS, GREEN_FOR_CANVAS_CROSS );
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
	if ( handle != 0 ) {
		enableTimer( SPEED );
	}

	return (handle != 0);
}

void CViewerWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );

	::UpdateWindow( handle );

	MSG msg = {};
	BOOL getMessageResult = 0;

	while( (getMessageResult = ::GetMessage( &msg, NULL, 0, 0 )) != 0 ) {
		if( !::TranslateAccelerator( handle, haccel, &msg ) ) {
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}
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

	//case WM_LBUTTONDBLCLK:
	//	wndPtr->onMouseClick( msg, wParam, lParam);
	//	return 0;

	case WM_MOUSELEAVE:
	case WM_LBUTTONUP:
		wndPtr->onMouseMove( wParam, lParam );
		wndPtr->currentMovingState = TMovingState_Viewer::MSV_None;
		return 0;

	case WM_COMMAND:
		wndPtr->onCommand( wParam, lParam );
		return 0;

	case WM_TIMER:
		wndPtr->onTimer();
		return 0;

	default:
		return ::DefWindowProc( handle, msg, wParam, lParam );
	}
}

void CViewerWindow::onCreate() {
	haccel = ::LoadAccelerators( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDC_POWERSPACE ) );

	if( haccel == NULL ) {
		::MessageBox( NULL, L"ERROR LoadAccelerators", L"ERROR", MB_ICONERROR );
		::PostQuitMessage( NULL );
	}
}

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
		//Event All should be carefully changed with Tick, when the model of scripts will be defined
		auto scripts = pair.second->GetScripts( EventType::EventAll );
		
		updateColorWithBuffer( activeId, ColorBufferActionType::RestoreColor );
		if( !scripts.empty() ) {
			scriptEngine.RunScripts( pair.first, scripts, EventType::EventTick );
		}
		updateColorWithBuffer( activeId, ColorBufferActionType::SetColor );
	}

	RECT rect;
	::GetClientRect( handle, &rect );
	rect.top -= rect.bottom;
	rect.bottom *= 2;
	rect.left -= rect.right;
	rect.right *= 2;
	::InvalidateRect( handle, &rect, true );
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

	float cosine = (float)cos( viewport.GetAngle() );
	float sine = (float)sin( viewport.GetAngle() );

	int oldWinWidth = rect.right - rect.left;
	int oldWinHeight = rect.bottom + rect.left;
/*
	int x1 = (int)(oldWinHeight * sine);
	int y1 = (int)(oldWinHeight * cosine);
	int x2 = (int)(oldWinWidth * cosine + oldWinHeight * sine);
	int y2 = (int)(oldWinHeight * cosine - oldWinWidth * sine);
	int x3 = (int)(oldWinWidth * cosine);
	int y3 = (int)(-oldWinWidth * sine);
	*/
	int winWidth = oldWinWidth * 3;
	int winHeight = oldWinHeight * 3;

	HDC Memhdc = ::CreateCompatibleDC( hdc );
	HBITMAP Membitmap = ::CreateCompatibleBitmap( hdc, winWidth, winHeight );
	::SelectObject( Memhdc, Membitmap );

	HBRUSH canvasBrush = ::CreateHatchBrush( HS_CROSS, BLUE_FOR_CANVAS_CROSS );

	::FillRect( Memhdc, &paintStruct.rcPaint, canvasBrush );

	SetGraphicsMode( hdc, GM_ADVANCED );
	XFORM xForm;
	xForm.eM11 = (FLOAT)cosine;
	xForm.eM12 = (FLOAT)sine;
	xForm.eM21 = (FLOAT)-sine;
	xForm.eM22 = (FLOAT)cosine;
	xForm.eDx = (FLOAT)0.0;
	xForm.eDy = (FLOAT)0.0;
	::SetWorldTransform( hdc, &xForm );

	stage.ClipAndDrawObjects( Memhdc );

	POINT oldZero = viewport.GetZeroLocation();
	POINT zero;

	zero.x = static_cast<LONG>(oldZero.x * cosine + oldZero.y * sine + oldWinWidth);
	zero.y = static_cast<LONG>(-oldZero.x * sine + oldZero.y * cosine + oldWinHeight);

	viewport.SetZeroLocation( zero );

	BOOL result = ::BitBlt( hdc, -oldWinWidth, -oldWinHeight, winWidth, winHeight, Memhdc, -zero.x, -zero.y, SRCCOPY );
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
		::PostQuitMessage( NULL );
	}

	::ReleaseDC( handle, tmp );
	::DeleteDC( tmp );
}

void CViewerWindow::onMouseMove( const WPARAM wParam, const LPARAM lParam )
{
	// TODO: handle mouse moving
	// E.g. mouse is on the object => change the color

	if( !wParam & MK_LBUTTON ) 		{
		currentMovingState = TMovingState_Viewer::MSV_None;
	}

	POINT point = getMouseCoords( lParam );

	int topId = -1;

	for( auto pair : stage.GetObjects() ) {
		TBox curBox = pair.second->GetContainingBox();

		if( isPointInBox( curBox, point ) ) {
			topId = pair.second->GetId();
		}
	}

	POINT dPoint = { 0, 0 };

	dPoint.x = (point.x - prevPoint.x);
	dPoint.y = (point.y - prevPoint.y);
	prevPoint = point;

	switch( currentMovingState ) {
	case TMovingState_Viewer::MSV_None:
		prevPoint = point;
		::SetCursor( ::LoadCursor( 0, IDC_ARROW ) );
		return;

	case TMovingState_Viewer::MSV_MovingCanvas:
		canvasPoint.x += dPoint.x;
		canvasPoint.y += dPoint.y;
		viewport.SetZeroLocation( canvasPoint );
		::SetCursor( ::LoadCursor( 0, IDC_SIZEALL ) );
		break;
	}
	
	RECT rect;
	::GetClientRect( handle, &rect );
	rect.top -= rect.bottom;
	rect.bottom *= 2;
	rect.left -= rect.right;
	rect.right *= 2;
	::InvalidateRect( handle, &rect, true );

	/*
	in this case we can somehow mark the object, but it looks terrible

	IDrawablePtr topObj = stage.GetObjectById( topId );

	COLORREF color = topObj->GetColor();
	topObj->SetColor( RGB( 0, 0, 0 ) );

	RECT rect;
	GetClientRect( handle, &rect );
	InvalidateRect( handle, &rect, false );

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
	if( !viewerIsRunning ) 		{
		if( wParam & MK_LBUTTON ) 			{
			currentMovingState = TMovingState_Viewer::MSV_MovingCanvas;
		}

		return;
	}

	UNREFERENCED_PARAMETER( msg );

	POINT mouseCoords = getMouseCoords( lParam );

	prevPoint = mouseCoords;

	int prevActiveId = activeId;
	activeId = 0;

	for( auto pair : stage.GetObjects() ) {
		TBox curBox = pair.second->GetContainingBox();

		if( isPointInBox( curBox, mouseCoords ) ) {
			activeId = pair.second->GetId();
		}
	}

	if( activeId == 0 &&
		wParam & MK_LBUTTON ) {
		currentMovingState = TMovingState_Viewer::MSV_MovingCanvas;
	}
	else 		{
		currentMovingState = TMovingState_Viewer::MSV_None;
	}

	// same active object as before => no action needed
	// // Strange behaviour. May be I want to change cycled colors,
	// // this way, I have to change focus every time. 
	// // Following code will be commented until the discussion.
	// if( activeId == prevActiveId )
	//	 return;
	
	

	// clicked on new object => have to process it
	updateColorWithBuffer( prevActiveId, ColorBufferActionType::RestoreColor );
	if( activeId != 0 ) {
		//Event All should be carefully changed with click, when the model of scripts will be defined
		auto scripts = stage.GetObjectById( activeId )->GetScripts( EventType::EventAll );
		if (!scripts.empty()) {
			scriptEngine.RunScripts( activeId, scripts, EventType::EventClick );
		}
	}
	updateColorWithBuffer( prevActiveId, ColorBufferActionType::SetColor );

	RECT rect;
	::GetClientRect( handle, &rect );
	rect.top -= rect.bottom;
	rect.bottom *= 2;
	rect.left -= rect.right;
	rect.right *= 2;
	::InvalidateRect( handle, &rect, true );
}

void CViewerWindow::onCommand( WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );

	HMENU pMenu = ::GetMenu( handle );

	switch LOWORD( wParam )
	{
	case ID_CONTROL_PLAY:
		viewerIsRunning = !viewerIsRunning;
		if( viewerIsRunning ) {
			enableTimer( SPEED );
		} else {
			disableTimer();
		}

		if( pMenu != NULL ) {
			if( viewerIsRunning )
				::CheckMenuItem( pMenu, ID_CONTROL_PLAY, MF_UNCHECKED | MF_BYCOMMAND );
			else
				::CheckMenuItem( pMenu, ID_CONTROL_PLAY, MF_CHECKED | MF_BYCOMMAND );
		} else {
			::MessageBox( handle, L"Menu not found", L"Error", MB_ICONERROR );
			::PostQuitMessage( NULL );
		}
		break;

	case ID_CONTROL_SPEEDUP:
		disableTimer();
		SPEED = static_cast<UINT>( SPEED * SPEED_MULTIPLIER );
		enableTimer( SPEED );
		break;

	case ID_CONTROL_SPEEDDOWN:
		disableTimer();
		SPEED = static_cast<UINT>(SPEED / SPEED_MULTIPLIER);
		enableTimer( SPEED );
		break;
	}

	::SetMenu( handle, pMenu );
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

void CViewerWindow::enableTimer( int timeDelay, int timerId)
{
	::SetTimer( handle, timerId, timeDelay, 0 );
}

void CViewerWindow::disableTimer( int timerId)
{
	::KillTimer( handle, timerId );
}

void CViewerWindow::updateColorWithBuffer( int prevActiveId, ColorBufferActionType actionType )
{
	switch ( actionType )
	{
		case ColorBufferActionType::RestoreColor:
		{
			if ( prevActiveId != 0 ) {
				stage.GetObjectById( prevActiveId )->SetColor( colorBuf );
			}
			break;
		}
		case ColorBufferActionType::SetColor:
		{
			if ( activeId != 0 ) {
				colorBuf = stage.GetObjectById( activeId )->GetColor( );
				stage.GetObjectById( activeId )->SetColor( static_cast<COLORREF> (colorBuf * 0.9) );
			}
			break;
		}
	}
}