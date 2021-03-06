#include <stdafx.h>
#include <Winuser.h>
#include "EditorRenderingWindow.h"
#include "resource.h"

#define MAX_RESOURCE_LENGTH 100

const int CEditorRenderingWindow::defaultHeight = 600;
const int CEditorRenderingWindow::defaultWidth = 1000;
const int CEditorRenderingWindow::markerHalfSize = 5;
const int CEditorRenderingWindow::rotateMarkerShift = 20;
const COLORREF CEditorRenderingWindow::backgroundColor = RGB( 255, 255, 255 );
const COLORREF CEditorRenderingWindow::markerColor = RGB( 0, 0, 255 );
const COLORREF CEditorRenderingWindow::borderColor = RGB( 0, 0, 0 );

Marker::Marker( const RECT& location, const TMarkerType type, const int id ) :
    location( location ), type( type ), index( id )
{}

const TMarkerType & Marker::GetType() const
{
    return type;
}

const RECT & Marker::GetLocation() const
{
    return location;
}

const int Marker::GetIndex() const
{
    return index;
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
    case TMarkerType::MT_Rotate:
        return IDC_HAND;
    default:
        assert( false );
        return 0;
    }
}

CEditorRenderingWindow::CEditorRenderingWindow() :
    bitmap( 0 ), bitmapContext( 0 ), bitmapHeight( 1 ), bitmapWidth( 1 ), backgroundBrush( 0 ), currentMovingState( MS_None )
{
    canvasPoint.x = 0;
    canvasPoint.y = 0;
}

bool CEditorRenderingWindow::Create( HWND hWndParent, const wchar_t * classname )
{
    HINSTANCE instance = GetModuleHandleW( nullptr );
    wchar_t title[MAX_RESOURCE_LENGTH];
    ::LoadString( instance, IDS_APP_TITLE, title, MAX_RESOURCE_LENGTH );
    this->handle = CreateWindowEx( 0, classname, title, WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, hWndParent, 0, instance, this );

    // Will be destroyed in OnDestroy
    markerPen = CreatePen( PS_SOLID, 1, markerColor );
    markerBrush = CreateSolidBrush( markerColor );
    backgroundBrush = CreateSolidBrush( backgroundColor );
    borderPen = CreatePen( PS_SOLID, 1, borderColor );

    return (handle != 0);
}

void CEditorRenderingWindow::Show( int cmdShow ) const
{
    ::ShowWindow( handle, cmdShow );
}

HWND CEditorRenderingWindow::GetHandle() const
{
    return handle;
}

void CEditorRenderingWindow::Redraw() const
{
    RECT rect;
    GetClientRect( handle, &rect );
    InvalidateRect( handle, &rect, true );
}

void CEditorRenderingWindow::OnDestroy()
{
    destroyDoubleBuffer();
    DeleteObject( backgroundBrush );
    DeleteObject( markerBrush );
    DeleteObject( markerPen );
    DeleteObject( borderPen );

    PostQuitMessage( 0 );
}

void CEditorRenderingWindow::DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const IdType& id, const double angle )
{
    SelectObject( paintDC, GetStockObject( NULL_BRUSH ) );
    if( id == selectedId ) {
        SelectObject( paintDC, markerPen );
        Rectangle( paintDC, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom );
    }

    rectangles.push_back( rectangle );
    rectanglesIds.push_back( id );
    angles.push_back( angle );

    if( id == selectedId ) {
        addMarkersForRectangle( paintDC, rectangle.left, rectangle.top, rectangle.right - rectangle.left,
            rectangle.bottom - rectangle.top, id, static_cast<int>(rectangles.size() - 1) );
    }
    SelectObject( paintDC, borderPen );
}

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
    {
        wndPtr->OnDestroy();
        break;
    }
    case WM_PAINT:
    {
        wndPtr->onPaint();
        return 0;
    }
    case WM_SIZING:
    {
        result = DefWindowProc( handle, message, wParam, lParam );
        wndPtr->onResize( reinterpret_cast<RECT*>(lParam) );
        return result;
    }
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
        ::SetFocus( handle );
        return result;
    }
    case WM_MOUSEMOVE:
    {
        wndPtr->onMouseMove( wParam, lParam );
        break;
    }
    case WM_MOUSEWHEEL:
    {
        wndPtr->onMouseWheel( wParam );
        break;
    }
    case WM_LBUTTONDOWN:
    {
        wndPtr->onMouseDown( lParam );
        break;
    }
    case WM_LBUTTONDBLCLK:
    {
        MessageBox(NULL, (LPCTSTR)"������� ����", (LPCWSTR)"Inform",
            MB_OK | MB_ICONINFORMATION);
        wndPtr->onDoubleClick( lParam );
        break;
    }
    case WM_MOUSELEAVE:
    case WM_LBUTTONUP:
    {
        wndPtr->onMouseUpOrLeave( lParam );
        break;
    }
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
    angles.clear();
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
    long dx = (point.x - lastPoint.x);
    long dy = (point.y - lastPoint.y);
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
        MoveDrawableObject( selectedId, resizeRect( point ) );
        break;
    case TMovingState::MS_Moving:
        lastSize.left += dx;
        lastSize.right += dx;
        lastSize.top += dy;
        lastSize.bottom += dy;
        MoveDrawableObject( selectedId, lastSize );
        SetCursor( LoadCursor( 0, IDC_SIZEALL ) );
        break;
    case TMovingState::MS_Rotating:
        lastAngle += getAngleBetweenPoints(lastPoint, point, rectCentre);
        RotateDrawableObject( selectedId, lastAngle );
        break;
    }
    lastPoint = point;
    Redraw();
}

void CEditorRenderingWindow::onMouseWheel( WPARAM wParam )
{
    Scaling( static_cast<signed short>(HIWORD( wParam )) / WHEEL_DELTA );
    Redraw();
}

void CEditorRenderingWindow::onMouseDown( const LPARAM lparam )
{
    POINT point = getPointByLParam( lparam );
    for( int i = static_cast<int>(markers.size()) - 1; i >= 0; i-- ) {
        if( contains( markers[i].GetLocation(), point ) ) {
            int index = markers[i].GetIndex();
            sizingMarkerType = markers[i].GetType();
            if( sizingMarkerType == TMarkerType::MT_Rotate ) {
                currentMovingState = TMovingState::MS_Rotating;
                SelectDrawableObject( rectanglesIds[index] );
                startAngle = angles[index];
                lastAngle = startAngle;
                rectCentre = getRectangleCentre( rectangles[index] );
                selectedId = rectanglesIds[index];
            } else {
                currentMovingState = TMovingState::MS_Sizing;
                SelectDrawableObject( rectanglesIds[index] );
                startSize = rectangles[index];
                lastSize = startSize;
                selectedId = rectanglesIds[index];
            }
            Redraw();
            return;
        }
    }
    for( int i = static_cast<int>(rectangles.size()) - 1; i >= 0; i-- ) {
        if( contains( rectangles[i], point ) ) {
            currentMovingState = TMovingState::MS_Moving;
            startSize = rectangles[i];
            lastSize = startSize;
            SelectDrawableObject( rectanglesIds[i] );
            selectedId = rectanglesIds[i];
            Redraw();
            return;
        }
    }
    lastPoint = point;
    selectedId = CObjectIdGenerator::GetEmptyId();
    SelectDrawableObject( CObjectIdGenerator::GetEmptyId() );
    currentMovingState = TMovingState::MS_MovingCanvas;
}

void CEditorRenderingWindow::onMouseUpOrLeave( const LPARAM lparam )
{
    onMouseMove( lparam );
    currentMovingState = TMovingState::MS_None;
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

void CEditorRenderingWindow::addMarkersForRectangle( HDC paintDC, const int x, const  int y, const int width, const int height, const IdType& id, const int index )
{
    addMarker( paintDC, x, y, TMarkerType::MT_LeftTop, id, index );
    addMarker( paintDC, x + width / 2, y, TMarkerType::MT_Top, id, index );
    addMarker( paintDC, x + width, y, TMarkerType::MT_RightTop, id, index );
    addMarker( paintDC, x + width, y + height / 2, TMarkerType::MT_Right, id, index );
    addMarker( paintDC, x + width, y + height, TMarkerType::MT_RightBottom, id, index );
    addMarker( paintDC, x + width / 2, y + height, TMarkerType::MT_Bottom, id, index );
    addMarker( paintDC, x, y + height, TMarkerType::MT_LeftBottom, id, index );
    addMarker( paintDC, x, y + height / 2, TMarkerType::MT_Left, id, index );
    addMarker( paintDC, x + width / 2, y - rotateMarkerShift, TMarkerType::MT_Rotate, id, index );
}

void  CEditorRenderingWindow::addMarker( HDC paintDC, const int x, const int y, const TMarkerType type, const IdType& id, const int index )
{
    RECT location;
    location.left = x - markerHalfSize;
    location.top = y - markerHalfSize;
    location.right = x + markerHalfSize;
    location.bottom = y + markerHalfSize;
    FillRect( paintDC, &location, markerBrush );
    markers.emplace_back( location, type, index );
    UNREFERENCED_PARAMETER( id );
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
    for( int i = static_cast<int>(markers.size()) - 1; i >= 0; i-- ) {
        if( contains( markers[i].GetLocation(), point ) ) {
            return markers[i].GetCursor();
        }
    }
    return IDC_ARROW;
}

RECT CEditorRenderingWindow::resizeRect( const POINT & point )
{
    RECT newSize = lastSize;
    switch( sizingMarkerType ) {
    case TMarkerType::MT_Left:
        newSize.left = min( startSize.right, point.x );
        break;
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

bool CEditorRenderingWindow::contains( const RECT & rect, const POINT & point )
{
    return ((point.x <= rect.right) && (point.x >= rect.left) && (point.y >= rect.top) && (point.y <= rect.bottom));
}

POINT CEditorRenderingWindow::getPointByLParam( const LPARAM & lparam )
{
    POINT result;
    result.x = LOWORD( lparam );
    result.y = HIWORD( lparam );
    return result;
}

double CEditorRenderingWindow::getAngleBetweenPoints( const POINT& point_start, const POINT& point_end, const POINT& point_centre ) const
{
    POINT vector_start = { point_start.x - point_centre.x, point_start.y - point_centre.y };
    POINT vector_end = { point_end.x - point_centre.x, point_end.y - point_centre.y };
    double cosinus = (vector_start.x * vector_end.x + vector_start.y * vector_end.y) /
        (sqrt( pow( vector_start.x, 2.0 ) + pow( vector_start.y, 2.0 ) ) * sqrt( pow( vector_end.x, 2.0 ) + pow( vector_end.y, 2.0 ) ));
    double point_against_line = vector_end.y * vector_start.x - vector_start.y * vector_end.x;
    if( point_against_line > 0 ) {
        return acos( cosinus );
    } else {
        return -acos( cosinus );
    }
}

POINT CEditorRenderingWindow::getRectangleCentre( const RECT& rect ) const
{
    POINT centre = { (rect.left + rect.right) / 2, (rect.bottom + rect.top) / 2 };
    return centre;
}

void CEditorRenderingWindow::onDoubleClick(const LPARAM lparam)
{
    POINT point = getPointByLParam(lparam);
    for (int i = static_cast<int>(markers.size()) - 1; i >= 0; i--) {
        if (contains(markers[i].GetLocation(), point)) {
            int index = markers[i].GetIndex();
            sizingMarkerType = markers[i].GetType();
            if (sizingMarkerType == TMarkerType::MT_Rotate) {
                currentMovingState = TMovingState::MS_Rotating;
                SelectDrawableObject(rectanglesIds[index]);
                startAngle = angles[index];
                lastAngle = startAngle;
                rectCentre = getRectangleCentre(rectangles[index]);
                selectedId = rectanglesIds[index];
            }
            else {
                currentMovingState = TMovingState::MS_Sizing;
                SelectDrawableObject(rectanglesIds[index]);
                startSize = rectangles[index];
                lastSize = startSize;
                selectedId = rectanglesIds[index];
            }
            Redraw();
            return;
        }
    }
    for (int i = static_cast<int>(rectangles.size()) - 1; i >= 0; i--) {
        if (contains(rectangles[i], point)) {
            currentMovingState = TMovingState::MS_Moving;
            startSize = rectangles[i];
            lastSize = startSize;
            SelectDrawableObject(rectanglesIds[i]);
            selectedId = rectanglesIds[i];
            Redraw();
            return;
        }
    }
    lastPoint = point;
    selectedId = CObjectIdGenerator::GetEmptyId();
    SelectDrawableObject(CObjectIdGenerator::GetEmptyId());
    currentMovingState = TMovingState::MS_MovingCanvas;
    wchar_t firstLetter = selectedId[0];
    if (firstLetter == L't') {
        DialogBox( GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)dialogProc );
        ShowWindow(handle, SW_SHOW);
    }
}

INT_PTR CEditorRenderingWindow::dialogProc(HWND hwnd, UINT msg, WPARAM wParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
        {
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDOK:
            {
                char buf[250];
                GetDlgItemText(hwnd, IDC_EDIT1, (LPWSTR)buf, sizeof(buf));

                return FALSE;
            }
            case IDCANCEL:
            {
                EndDialog(hwnd, 0);
                return FALSE;
            }
            }
        }
        case WM_CLOSE:
        {
            EndDialog(hwnd, 0);
            return FALSE;
        }
    }
    return FALSE;
}