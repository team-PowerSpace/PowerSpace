#include "stdafx.h"
#include "RenderingWindow.h"
#include "Resource.h"

#define MAX_RESOURCE_LENGTH 100

CRenderingWindow::CRenderingWindow() : opacity( 0.5f ), centerX( 0 ), centerY( 0 ), scaleX( 1.0f ), scaleY( 1.0f ),
bitmap( 0 ), bitmapContext( 0 ), bitmapHeight( 1 ), bitmapWidth( 1 ),
backgroundBrush( ::CreateSolidBrush( BackgroundColor ) )
{
}

CRenderingWindow::~CRenderingWindow()
{
	// This destructor is virtual thats why it is essential to create this code 
}

bool CRenderingWindow::RegisterClass()
{
	HBRUSH backgroundBrush = reinterpret_cast<HBRUSH> (::GetStockObject( WHITE_BRUSH ));
	HMODULE instance = GetModuleHandleW( nullptr );

	WNDCLASSEX windowClassInforamtion;
	windowClassInforamtion.cbSize = sizeof( WNDCLASSEX );
	windowClassInforamtion.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassInforamtion.lpfnWndProc = windowProc;
	windowClassInforamtion.cbClsExtra = 0;
	windowClassInforamtion.cbWndExtra = 2 * sizeof( LONG_PTR );
	windowClassInforamtion.hInstance = instance;
	windowClassInforamtion.hIcon = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );
	windowClassInforamtion.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClassInforamtion.hbrBackground = backgroundBrush;
	windowClassInforamtion.lpszMenuName = 0;
	windowClassInforamtion.lpszClassName = ClassName;
	windowClassInforamtion.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );

	return (::RegisterClassEx( &windowClassInforamtion ) == 0 ? false : true);
}

bool CRenderingWindow::Create()
{
	HINSTANCE instance = GetModuleHandleW( nullptr );
	wchar_t title[MAX_RESOURCE_LENGTH];
	::LoadString( instance, IDS_APP_TITLE, title, MAX_RESOURCE_LENGTH );
	handle = CreateWindowEx( 0, CRenderingWindow::ClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		DefaultWidth, DefaultHeight, 0, 0, instance, this );
	return (handle != 0);
}

void CRenderingWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CRenderingWindow::GetHandle() const
{
	return handle;
}

void CRenderingWindow::OnDestroy()
{
	destroyDoubleBuffer();
	::DeleteObject( backgroundBrush );
	::PostQuitMessage( 0 );
}

const wchar_t* CRenderingWindow::ClassName = L"CRenderingWindow";
const int CRenderingWindow::DefaultHeight = 600;
const int CRenderingWindow::DefaultWidth = 1000;
const COLORREF CRenderingWindow::BackgroundColor = RGB( 255, 255, 255 );

LRESULT CRenderingWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );
		return DefWindowProc( handle, message, wParam, lParam );
	}

	CRenderingWindow* wndPtr = reinterpret_cast<CRenderingWindow*>(GetWindowLongPtr( handle, 0 ));
	LRESULT result;
	switch( message ) {
		case WM_DESTROY:
			wndPtr->OnDestroy();
			break;
		case WM_PAINT:
			wndPtr->onPaint();
			return 0;
		case WM_SIZING:
			result = DefWindowProc( handle, message, wParam, lParam );
			wndPtr->onResize( reinterpret_cast<RECT*>(lParam) );
			return result;
		case WM_SIZE:
		{
			result = DefWindowProc( handle, message, wParam, lParam );
			RECT area;
			::GetClientRect( handle, &area );
			wndPtr->onResize( &area );
			return result;
		}
		case WM_CREATE:
		{
			result = DefWindowProc( handle, message, wParam, lParam );
			RECT area;
			::GetClientRect( handle, &area );
			wndPtr->onResize( &area );
			return result;
		}
		case WM_MOUSEMOVE:
			wndPtr->onMouseMove( wParam, lParam );
			break;
	}
	return DefWindowProc( handle, message, wParam, lParam );

}

void CRenderingWindow::onPaint() const
{
	PAINTSTRUCT paintStruct;
	HDC paintDC = ::BeginPaint( handle, &paintStruct );

	RECT rect;
	::GetClientRect( handle, &rect );

	drawContent( bitmapContext, bitmapWidth, bitmapHeight );
	BOOL code = BitBlt( paintDC, 0, 0, bitmapWidth, bitmapHeight, bitmapContext, 0, 0, SRCCOPY );
	if( !code ) {
		::MessageBox( handle, L"Bad paint", L"DEBUG", MB_ICONERROR );
		PostQuitMessage( 4 );
	}

	::EndPaint( handle, &paintStruct );
}

#pragma warning(push)
#pragma warning(disable:4100)
void CRenderingWindow::onMouseMove( const WPARAM wParam, const LPARAM lParam )
{
}
#pragma warning (pop)

void CRenderingWindow::onResize( const RECT * area )
{
	destroyDoubleBuffer();
	HDC tmp = GetDC( handle );
	bitmapContext = CreateCompatibleDC( tmp );
	bitmapWidth = area->right - area->left + 1;
	bitmapHeight = area->bottom - area->top + 1;
	centerX = bitmapWidth / 2.0f;
	centerY = bitmapHeight / 2.0f;
	bitmap = CreateCompatibleBitmap( tmp, bitmapWidth, bitmapHeight );
	SelectObject( bitmapContext, bitmap );
	if( SetGraphicsMode( bitmapContext, GM_ADVANCED ) == 0 ) {
		MessageBox( handle, L"Error. Can't set graphycs mode", L"Error", MB_ICONERROR | MB_OK );
	}
	ReleaseDC( handle, tmp );
}

void CRenderingWindow::drawContent( HDC paintDC, const int width, const int height ) const
{
	drawEraseRectangle( paintDC, width, height );
}

void CRenderingWindow::drawEraseRectangle( HDC paintDC, const int width, const int height ) const
{
	RECT clientContext;
	clientContext.bottom = height;
	clientContext.right = width;
	clientContext.left = 0;
	clientContext.top = 0;
	FillRect( paintDC, &clientContext, backgroundBrush );
}

void CRenderingWindow::destroyDoubleBuffer()
{
	if( (bitmapContext != 0) || (bitmap != 0) ) {
		DeleteDC( bitmapContext );
		DeleteObject( bitmap );
	}
}

