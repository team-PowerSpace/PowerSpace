#include <stdafx.h>
#include "EditorRenderingWindow.h"
#include "Resource.h"
#include <Winuser.h>

#define MAX_RESOURCE_LENGTH 100

CEditorRenderingWindow::CEditorRenderingWindow() :
	bitmap( 0 ), bitmapContext( 0 ), bitmapHeight( 1 ), bitmapWidth( 1 ), backgroundBrush( 0 ),
	markerBrush( 0 ), markerPen( 0 ), currentMovingState( MS_None )
{
	canvasPoint.x = 0;
	canvasPoint.y = 0;
}

CEditorRenderingWindow::~CEditorRenderingWindow()
{
	// This destructor is virtual thats why it is essential to create this code 
}


void CEditorRenderingWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CEditorRenderingWindow::GetHandle() const
{
	return handle;
}

bool CEditorRenderingWindow::Create( const wchar_t * classname )
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

void CEditorRenderingWindow::OnDestroy()
{
	destroyDoubleBuffer();
	::DeleteObject( backgroundBrush );
	::DeleteObject( markerBrush );
	::DeleteObject( markerPen );

	::PostQuitMessage( 0 );
}

const int CEditorRenderingWindow::DefaultHeight = 600;
const int CEditorRenderingWindow::DefaultWidth = 1000;
const int CEditorRenderingWindow::MarkerHalfSize = 5;
const COLORREF CEditorRenderingWindow::BackgroundColor = RGB( 255, 255, 255 );
const COLORREF CEditorRenderingWindow::MarkerColor = RGB( 0, 0, 255 );

LRESULT CEditorRenderingWindow::WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(
			reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );
		return DefWindowProc( handle, message, wParam, lParam );
	}

	CEditorRenderingWindow* wndPtr = reinterpret_cast<CEditorRenderingWindow*>(GetWindowLongPtr( handle, 0 ));
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
		case WM_MOUSEWHEEL:
			wndPtr->onMouseWheel( wParam );
			break;
		case WM_LBUTTONDOWN:
			wndPtr->onMouseDown( lParam );
			break;
		case WM_MOUSELEAVE:
		case WM_LBUTTONUP:
			wndPtr->onMouseUpOrLeave( lParam );
			break;
		default:
			// Will be returned DefWindowProc
			break;
	}
	return DefWindowProc( handle, message, wParam, lParam );

}

void CEditorRenderingWindow::onPaint()
{
	PAINTSTRUCT paintStruct;
	HDC paintDC = ::BeginPaint( handle, &paintStruct );

	RECT rect;
	::GetClientRect( handle, &rect );

	drawEraseRectangle( bitmapContext, bitmapWidth, bitmapHeight );

	rectanglesIds.clear();
	rectangles.clear();
	markers.clear();
	DrawContent( bitmapContext, bitmapWidth, bitmapHeight );


	BOOL code = BitBlt( paintDC, 0, 0, bitmapWidth, bitmapHeight, bitmapContext, 0, 0, SRCCOPY );
	if( !code ) {
		::MessageBox( handle, L"Bad paint", L"DEBUG", MB_ICONERROR );
		PostQuitMessage( 4 );
	}

	::EndPaint( handle, &paintStruct );
}

void CEditorRenderingWindow::onMouseMove( const WPARAM wParam, const LPARAM lParam )
{
	onMouseMove( lParam );
	if( !(wParam&MK_LBUTTON) ) {
		currentMovingState = TMovingState::MS_None;
	}
}

void CEditorRenderingWindow::onMouseMove( const LPARAM lParam )
{
	POINT point = getPointByLParam( lParam );
	long dx = (point.x - lastPoint.y);
	long dy = (point.y - lastPoint.y);
	lastPoint = point;
	// Actions which reuire redraw finished by break, others by return
	switch( currentMovingState ) {
		case TMovingState::MS_None:
			lastPoint = point;
			SetCursor( LoadCursor( 0, getCursor( point ) ) );
			return;
		case TMovingState::MS_MovingCanvas:
			canvasPoint.x += dx;
			canvasPoint.y += dy;
			MoveCanvas( canvasPoint );
			SetCursor( LoadCursor( 0, IDC_SIZEALL ) );
			break;
		case TMovingState::MS_Sizing:
			MoveRectangle( selectedId, resizeRect( point ) );
			break;
		case TMovingState::MS_Moving:
			lastSize.left += dx;
			lastSize.right += dx;
			lastSize.top += dy;
			lastSize.bottom += dy;
			MoveRectangle( selectedId, lastSize );
			SetCursor( LoadCursor( 0, IDC_SIZEALL ) );
			break;
	}
	reDraw();

}

void CEditorRenderingWindow::onResize( const RECT * area )
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

void CEditorRenderingWindow::drawEraseRectangle( HDC paintDC, const int width, const int height ) const
{
	RECT clientContext;
	clientContext.bottom = height;
	clientContext.right = width;
	clientContext.left = 0;
	clientContext.top = 0;
	FillRect( paintDC, &clientContext, backgroundBrush );
}

void CEditorRenderingWindow::DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const int id )
{
	::Rectangle( paintDC, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom );

	rectangles.push_back( rectangle );
	rectanglesIds.push_back( id );

	addMarkersForRectangle( paintDC, rectangle.left, rectangle.top, rectangle.right - rectangle.left,
		rectangle.top - rectangle.bottom, id );
}

void CEditorRenderingWindow::addMarkersForRectangle( HDC paintDC, const int x, const  int y, const int width, const  int height, const int id )
{
	addMarker( paintDC, x, y, TMarkerType::MT_LeftTop, id );
	addMarker( paintDC, x + width / 2, y, TMarkerType::MT_Top, id );
	addMarker( paintDC, x + width, y, TMarkerType::MT_RightTop, id );
	addMarker( paintDC, x + width, y + height / 2, TMarkerType::MT_Right, id );
	addMarker( paintDC, x + width, y + height, TMarkerType::MT_RightBottom, id );
	addMarker( paintDC, x + width / 2, y + height, TMarkerType::MT_Bottom, id );
	addMarker( paintDC, x, y + height, TMarkerType::MT_LeftBottom, id );
	addMarker( paintDC, x, y + height / 2, TMarkerType::MT_Left, id );
}


void  CEditorRenderingWindow::addMarker( HDC paintDC, const int x, const int y, const TMarkerType type, const int id )
{
	RECT location;
	location.left = x - MarkerHalfSize;
	location.top = y - MarkerHalfSize;
	location.right = x + MarkerHalfSize;
	location.bottom = y + MarkerHalfSize;
	::FillRect( paintDC, &location, markerBrush );
	markers.emplace_back( location, type, id );
}

void CEditorRenderingWindow::destroyDoubleBuffer()
{
	if( (bitmapContext != 0) || (bitmap != 0) ) {
		DeleteDC( bitmapContext );
		DeleteObject( bitmap );
	}
}

LPCTSTR CEditorRenderingWindow::getCursor( const POINT & point ) const
{
	for( int i = markers.size() - 1; i >= 0; i-- ) {
		if( contains( markers[i].GetLocation(), point ) ) {
			return markers[i].GetCursor();
		}
	}
	for( int i = rectangles.size() - 1; i >= 0; i-- ) {
		if( contains( rectangles[i], point ) ) {
			return IDC_CROSS;
		}
	}
	return IDC_ARROW;
}

void CEditorRenderingWindow::reDraw() const
{
	RECT rect;
	GetClientRect( handle, &rect );
	InvalidateRect( handle, &rect, true );
}

RECT CEditorRenderingWindow::resizeRect( const POINT & point )
{
	RECT newSize = lastSize;
	switch( sizingMarkerType ) {
		case TMarkerType::MT_Left:
			newSize.left = min( startSize.right, point.x );
		case TMarkerType::MT_Top:
			newSize.top = min( startSize.bottom, point.y );
			break;
		case TMarkerType::MT_Bottom:
			newSize.bottom = startSize.top + max( 0, point.y - startSize.top );
			break;
		case TMarkerType::MT_Right:
			newSize.right = startSize.left + max( 0, point.x - startSize.left );
			break;
		case TMarkerType::MT_LeftTop:
			newSize.left = min( startSize.right, point.x );
			newSize.top = min( startSize.bottom, point.y );
			break;
		case TMarkerType::MT_LeftBottom:
			newSize.left = min( startSize.right, point.x );
			newSize.bottom = startSize.top + max( 0, point.y - startSize.top );
			break;
		case TMarkerType::MT_RightBottom:
			newSize.bottom = startSize.top + max( 0, point.y - startSize.top );
			newSize.right = startSize.left + max( 0, point.x - startSize.left );
			break;
		case TMarkerType::MT_RightTop:
			newSize.top = min( startSize.bottom, point.y );
			newSize.right = startSize.left + max( 0, point.x - startSize.left );
			break;
	}
	lastSize = newSize;
	return newSize;
}

void CEditorRenderingWindow::onMouseWheel( WPARAM wParam )
{
	Scaling( HIWORD( wParam ) / WHEEL_DELTA );
}

bool CEditorRenderingWindow::contains( const RECT & rect, const POINT & point )
{
	return ((point.x <= rect.right) && (point.x >= rect.left) && (point.y >= rect.top) && (point.y <= rect.bottom));
}

void CEditorRenderingWindow::onMouseDown( const LPARAM lparam )
{
	POINT point = getPointByLParam( lparam );
	for( int i = markers.size() - 1; i >= 0; i-- ) {
		if( contains( markers[i].GetLocation(), point ) ) {
			int id = markers[i].GetId();
			sizingMarkerType = markers[i].GetType();
			currentMovingState = TMovingState::MS_Sizing;
			SelectRectangle( id );
			startSize = rectangles[id];
			lastSize = startSize;
			reDraw();
			return;
		}
	}
	for( int i = rectangles.size() - 1; i >= 0; i-- ) {
		if( contains( rectangles[i], point ) ) {
			currentMovingState = TMovingState::MS_Moving;
			startSize = rectangles[i];
			lastSize = startSize;
			SelectRectangle( i );
			reDraw();
		}
	}
}

void CEditorRenderingWindow::onMouseUpOrLeave( const LPARAM lparam )
{
	onMouseMove( lparam );
	currentMovingState = TMovingState::MS_None;
}

POINT CEditorRenderingWindow::getPointByLParam( const LPARAM & lparam )
{
	POINT result;
	result.x = LOWORD( lparam );
	result.y = HIWORD( lparam );
	return result;
}

Marker::Marker( const RECT& location, const TMarkerType type, const int id ) :
	location( location ), type( type ), id( id )
{
}

const TMarkerType & Marker::GetType() const
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

LPCTSTR Marker::GetCursor() const
{
	switch( type ) {
		case TMarkerType::MT_Top:
		case TMarkerType::MT_Bottom:
			return IDC_SIZENS;
		case TMarkerType::MT_Left:
		case TMarkerType::MT_Right:
			return IDC_SIZEWE;
		case TMarkerType::MT_LeftBottom:
		case TMarkerType::MT_RightTop:
			return IDC_SIZENESW;
		case TMarkerType::MT_RightBottom:
		case TMarkerType::MT_LeftTop:
			return IDC_SIZENWSE;
		default:
			assert( false );
			return 0;
	}

}
