#include <stdafx.h>
#include "ViewerWindow.h"

const wchar_t* CViewerWindow::ClassName = L"CViewerWindow";
const UINT TICK_LENGTH = 10;

CViewerWindow::CViewerWindow( CStage& _stage, CViewport& _viewport, CCanvas& _canvas) :
	windowHeight( 600 ), windowWidth( 800 ), viewport(_viewport), canvas(_canvas), 
    handle(nullptr), stage(_stage), scriptEngine(stage)
{
}

CViewerWindow::~CViewerWindow()
{
}

//
//  FUNCTION: RegisterClass()
//
//  PURPOSE: Registers the viewer window class.
//
bool CViewerWindow::RegisterClass()
{
	HBRUSH backgroundBrush = reinterpret_cast<HBRUSH> (::GetStockObject( WHITE_BRUSH ));
	HMODULE instance = GetModuleHandleW( nullptr );

	WNDCLASSEX windowClassInformation;
	windowClassInformation.cbSize = sizeof( WNDCLASSEX );
	windowClassInformation.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassInformation.lpfnWndProc = WindowProc;
	windowClassInformation.cbClsExtra = 0;
	windowClassInformation.cbWndExtra = 2 * sizeof( LONG_PTR );
	windowClassInformation.hInstance = instance;
	windowClassInformation.hIcon = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );
	windowClassInformation.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClassInformation.hbrBackground = backgroundBrush;
	windowClassInformation.lpszMenuName = 0;
	windowClassInformation.lpszClassName = ClassName;
	windowClassInformation.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );

	return (::RegisterClassEx( &windowClassInformation ) != 0);
}

//
//  FUNCTION: Create()
//
//  PURPOSE: Creates the viewer window
//
bool CViewerWindow::Create()
{
	HINSTANCE hInstance = GetModuleHandle( nullptr );

	handle = CreateWindowEx( 0, ClassName, L"PowerSpace Viewer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowHeight, windowWidth, nullptr, nullptr, hInstance, this );

	return (handle != 0);
}

//
//  FUNCTION: WindowProc()
//
//  PURPOSE: Main window proc
//
LRESULT CViewerWindow::WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );

		return DefWindowProc( handle, msg, wParam, lParam );
	}

	CViewerWindow* wndPtr = reinterpret_cast<CViewerWindow*>(GetWindowLongPtr( handle, 0 ));

	switch( msg ) {
	case WM_CREATE:
		wndPtr->onCreate();
		return 0;

    case WM_CLOSE:
        DestroyWindow(handle);
        return 0;

	case WM_DESTROY:
		wndPtr->onDestroy();
		return 0;

	case WM_SIZE:
		wndPtr->onSize();
		return 0;

	case WM_PAINT:
		wndPtr->onPaint();
		return 0;

	case WM_MOUSEMOVE:
		wndPtr->onMouseMove( wParam, lParam );
		return 0;

	case WM_LBUTTONDOWN:
		wndPtr->onMouseClick( msg, wParam, lParam );
		return 0;

	default:
		return ::DefWindowProc( handle, msg, wParam, lParam );
	}
}

void CViewerWindow::onDestroy()
{
	::PostQuitMessage( 0 );
}

void CViewerWindow::onCreate()
{
	::SetTimer( handle, 1, TICK_LENGTH, nullptr );
}

//
//  FUNCTION: onTimer()
//
//  PURPOSE: HERE WE HANDLE THE ONTICK EVENTS
//
void CViewerWindow::onTimer()
{
//	tickHandler();
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

	::FillRect( Memhdc, &paintStruct.rcPaint, (HBRUSH)COLOR_WINDOW );

    stage.ClipAndDrawObjects( Memhdc );

	BOOL result = ::BitBlt( hdc, 0, 0, winWidth, winHeight, Memhdc, 0, 0, SRCCOPY );
	if( !result ) {
		::MessageBox( handle, L"BitBlt : onPaint() : CViewerWindow", L"Error", MB_ICONERROR );
		PostQuitMessage( NULL );
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
}

void CViewerWindow::onMouseMove( const WPARAM wParam, const LPARAM lParam )
{ 
	// TODO: handle mouse moving
	// E.g. mouse is on the object => change the color

	UNREFERENCED_PARAMETER( wParam );
	UNREFERENCED_PARAMETER( lParam );
}

//
//  FUNCTION: onMouseClick()
//
//  PURPOSE: HERE WE HANDLE THE ONCLICK EVENTS
//
void CViewerWindow::onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    UNREFERENCED_PARAMETER( wParam );
    UNREFERENCED_PARAMETER( msg );

	// handle mouse clicks on objects
    // todo: detect which object was clicked, now all the objects are activated
    for( auto pair : stage.GetObjects() ) {
        auto scripts = pair.second->GetScripts( EventType::EventClick );
        if( !scripts.empty() ) {
            scriptEngine.RunScripts(pair.first, scripts);
        }
    }
}

void CViewerWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CViewerWindow::GetHandle() const
{
	return handle;
}
