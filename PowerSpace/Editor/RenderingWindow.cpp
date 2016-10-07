#include "stdafx.h"
#include "RenderingWindow.h"
#include "Resource.h"
#include <Winuser.h>

#define MAX_RESOURCE_LENGTH 100

CRenderingWindow::CRenderingWindow() :
	bitmap( 0 ), bitmapContext( 0 ), bitmapHeight( 1 ), bitmapWidth( 1 ), backgroundBrush( 0 ),
	markerBrush( 0 ), markerPen( 0 )
{
}

CRenderingWindow::~CRenderingWindow()
{
	// This destructor is virtual thats why it is essential to create this code 
}


void CRenderingWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CRenderingWindow::GetHandle() const
{
	return handle;
}

bool CRenderingWindow::Create( const wchar_t * classname )
{
	HINSTANCE instance = GetModuleHandleW( nullptr );
	wchar_t title[MAX_RESOURCE_LENGTH];
	::LoadString( instance, IDS_APP_TITLE, title, MAX_RESOURCE_LENGTH );
	handle = CreateWindowEx( 0, classname, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		DefaultWidth, DefaultHeight, 0, 0, instance, this );

	// Will be destroyed in OnDestroy
	markerPen = CreatePen( PS_DASH, 1, MarkerColor );
	markerBrush = CreateSolidBrush( MarkerColor );
	backgroundBrush = CreateSolidBrush( BackgroundColor );

	return (handle != 0);
}

void CRenderingWindow::OnDestroy()
{
	destroyDoubleBuffer();
	::DeleteObject( backgroundBrush );
	::DeleteObject( markerBrush );
	::DeleteObject( markerPen );

	::PostQuitMessage( 0 );
}

const int CRenderingWindow::DefaultHeight = 600;
const int CRenderingWindow::DefaultWidth = 1000;
const int CRenderingWindow::MarkerHalfSize = 5;
const COLORREF CRenderingWindow::BackgroundColor = RGB( 255, 255, 255 );
const COLORREF CRenderingWindow::MarkerColor = RGB( 0, 0, 255 );

LRESULT CRenderingWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(
			reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );
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
		default:
			// Will be returned DefWindowProc
			break;
	}
	return DefWindowProc( handle, message, wParam, lParam );

}

void CRenderingWindow::onPaint()
{
	PAINTSTRUCT paintStruct;
	HDC paintDC = ::BeginPaint( handle, &paintStruct );

	RECT rect;
	::GetClientRect( handle, &rect );

	drawEraseRectangle( bitmapContext, bitmapWidth, bitmapHeight );

	rectanglesIds.clear();
	rectangles.clear();
	markers.clear();
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
	// TODO: impliment this
	// Planned functions:
	// a) Cursor changing
	// b) Moving of objects
	// c) Moving of canvas
}
#pragma warning (pop)

void CRenderingWindow::onResize( const RECT * area )
{
	destroyDoubleBuffer();
	HDC tmp = GetDC( handle );
	bitmapContext = CreateCompatibleDC( tmp );
	bitmapWidth = area->right - area->left + 1;
	bitmapHeight = area->bottom - area->top + 1;
	bitmap = CreateCompatibleBitmap( tmp, bitmapWidth, bitmapHeight );
	SelectObject( bitmapContext, bitmap );
	if( SetGraphicsMode( bitmapContext, GM_ADVANCED ) == 0 ) {
		MessageBox( handle, L"Error. Can't set graphycs mode", L"Error", MB_ICONERROR | MB_OK );
	}
	ReleaseDC( handle, tmp );
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

void CRenderingWindow::DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const int id )
{
	::Rectangle( paintDC, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom );

	rectangles.push_back( rectangle );
	rectanglesIds.push_back( id );

	AddMarkersForRectangle( paintDC, rectangle.left, rectangle.top, rectangle.right - rectangle.left,
		rectangle.top - rectangle.bottom, id );
}

void CRenderingWindow::AddMarkersForRectangle( HDC paintDC, const int x, const  int y, const int width, const  int height, const int id )
{
	AddMarker( paintDC, x, y, MarkerType::LeftTop, id );
	AddMarker( paintDC, x + width / 2, y, MarkerType::Top, id );
	AddMarker( paintDC, x + width, y, MarkerType::RightTop, id );
	AddMarker( paintDC, x + width, y + height / 2, MarkerType::Right, id );
	AddMarker( paintDC, x + width, y + height, MarkerType::RightBottom, id );
	AddMarker( paintDC, x + width / 2, y + height, MarkerType::Bottom, id );
	AddMarker( paintDC, x, y + height, MarkerType::LeftBottom, id );
	AddMarker( paintDC, x, y + height / 2, MarkerType::Left, id );
}


void  CRenderingWindow::AddMarker( HDC paintDC, const int x, const int y, const MarkerType type, const int id )
{
	RECT location;
	location.left = x - MarkerHalfSize;
	location.top = y - MarkerHalfSize;
	location.right = x + MarkerHalfSize;
	location.bottom = y + MarkerHalfSize;
	::FillRect( paintDC, &location, markerBrush );
	markers.emplace_back( location, type, id );
}

void CRenderingWindow::destroyDoubleBuffer()
{
	if( (bitmapContext != 0) || (bitmap != 0) ) {
		DeleteDC( bitmapContext );
		DeleteObject( bitmap );
	}
}

Marker::Marker( const RECT& location, const MarkerType type, const int id ) :
	location( location ), type( type ), id( id )
{
}

const MarkerType & Marker::GetType() const
{
	return type;
}

const RECT & Marker::GetLocation() const
{
	return location;
}

const int Marker::GetId() const
{
	return id;
}

DWORD Marker::GetCursor() const
{
	switch( type ) {
		case MarkerType::Top:
		case MarkerType::Bottom:
			return OCR_SIZENS;
		case MarkerType::Left:
		case MarkerType::Right:
			return OCR_SIZEWE;
		case MarkerType::LeftBottom:
		case MarkerType::RightTop:
			return OCR_SIZENESW;
		case MarkerType::RightBottom:
		case MarkerType::LeftTop:
			return OCR_SIZENWSE;
		default:
			assert( false );
			return 0;
	}

}
