#include <stdafx.h>
#include "ViewerWindow.h"

const wchar_t* CViewerWindow::ClassName = L"CViewerWindow";
const wchar_t* CViewerWindow::ViewerApplicationName = L"Powerspace Viewer";
const UINT TICK_LENGTH = 1000;
const double SPEED_MULTIPLIER = 1.1;
UINT SPEED = TICK_LENGTH;


CViewerWindow::CViewerWindow( CStage& _stage, CCanvas& _canvas ) :
    windowHeight( 600 ), windowWidth( 800 ), viewport( _stage.GetViewPort() ), canvas( _canvas ),
    handle( nullptr ), stage( _stage ), scriptEngine( stage ), activeId( CObjectIdGenerator::GetEmptyId() ), colorBuf( -1 ),
    viewerIsRunning( true ), currentMovingState( MSV_None )
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
        CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
        hInstance, this );
    if ( handle != 0 ) {
        enableTimer( SPEED );
    }

    return (handle != 0);
}

void CViewerWindow::Show() const
{
    ::ShowWindow( handle, SW_SHOW );

    ::UpdateWindow( handle );
}

void CViewerWindow::Redraw() const
{
    RECT rect;
    GetClientRect( handle, &rect );
    InvalidateRect( handle, &rect, true );
}

HWND CViewerWindow::GetHandle() const
{
    return handle;
}

void CViewerWindow::scaling( const int direction )
{
    viewport.SetScale( viewport.GetScale() * pow( scalingFactor, direction ) );
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
        ::SetFocus( handle );
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
    case WM_MOUSEWHEEL:
        wndPtr->onMouseWheel( wParam );
        return 0;
    case WM_LBUTTONDOWN:
        wndPtr->onMouseClick( msg, wParam, lParam );
        return 0;
    //case WM_LBUTTONDBLCLK:
    //    wndPtr->onMouseClick( msg, wParam, lParam);
    //    return 0;

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
        //auto scripts = pair.second->GetScripts( EventType::EventTick );
        auto scripts = stage.getScripts( activeId, EventType::EventTick );
        updateColorWithBuffer( activeId, ColorBufferActionType::RestoreColor );
        if( !scripts.empty() ) {
            scriptEngine.RunScripts( activeId, EventType::EventTick, scripts );
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

    int viewportHeight = rect.bottom - rect.top;
    int viewportWidth = rect.right - rect.left;

    HDC Memhdc = ::CreateCompatibleDC( hdc );
    HBITMAP Membitmap = ::CreateCompatibleBitmap( hdc, viewportWidth, viewportHeight );
    ::SelectObject( Memhdc, Membitmap );
    
    HBRUSH canvasBrush = ::CreateHatchBrush( HS_CROSS, BLUE_FOR_CANVAS_CROSS );

    ::FillRect( Memhdc, &paintStruct.rcPaint, canvasBrush );

    stage.ClipAndDrawObjects( Memhdc );

    POINT zero = viewport.GetZeroLocation();

    //BOOL result = ::BitBlt( hdc, paintStruct.rcPaint.left, paintStruct.rcPaint.top, 2*radius, 2*radius, Memhdc, paintStruct.rcPaint.left, paintStruct.rcPaint.top, SRCCOPY );
    BOOL result = ::BitBlt( hdc, zero.x, zero.y, viewportWidth, viewportHeight, Memhdc, 0, 0, SRCCOPY );
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

    if( !wParam & MK_LBUTTON )         {
        currentMovingState = TMovingState_Viewer::MSV_None;
    }

    POINT point = getMouseCoords( lParam ); // it's okay to use screen coords as we count only delta values

    IdType topId = CObjectIdGenerator::GetEmptyId();

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
        canvasPoint = viewport.GetZeroLocation();

        float cosine = static_cast<float>(cos( viewport.GetAngle() ));
        float sine = static_cast<float>(sin( viewport.GetAngle() ));

        canvasPoint.x += static_cast<LONG>(dPoint.x * cosine + dPoint.y * sine);
        canvasPoint.y += static_cast<LONG>(-dPoint.x * sine + dPoint.y * cosine);

        viewport.SetZeroLocation( canvasPoint );
        ::SetCursor( ::LoadCursor( 0, IDC_SIZEALL ) );
        break;
    }
    
    RECT rect;
    ::GetClientRect( handle, &rect );
    ::InvalidateRect( handle, &rect, false );
}

//
//  FUNCTION: onMouseClick()
//
//  PURPOSE: HERE WE HANDLE THE ONCLICK EVENTS
//
void CViewerWindow::onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam )
{
    if( !viewerIsRunning )         {
        if( wParam & MK_LBUTTON )             {
            currentMovingState = TMovingState_Viewer::MSV_MovingCanvas;
        }

        return;
    }

    UNREFERENCED_PARAMETER( msg );

    POINT mouseCoords = getMouseCoords( lParam );
    prevPoint = mouseCoords;
    // storage space for usual color of currently active object
    IdType prevActiveId = activeId;
    activeId = CObjectIdGenerator::GetEmptyId();
    bool changed = false;

    for( auto pair : stage.GetObjects() ) {
        TBox curBox = pair.second->GetContainingBox();

        if( isPointInBox( curBox, viewport.ConvertToModelCoordinates( mouseCoords ) ) ) {
            activeId = pair.second->GetId();
            changed = true;
        }
    }

    if( !changed &&
        wParam & MK_LBUTTON ) {
        currentMovingState = TMovingState_Viewer::MSV_MovingCanvas;
    }
    else         {
        currentMovingState = TMovingState_Viewer::MSV_None;
    }

    // same active object as before => no action needed
    // // Strange behaviour. May be I want to change cycled colors,
    // // this way, I have to change focus every time. 
    // // Following code will be commented until the discussion.
    // if( activeId == prevActiveId )
    //     return;

    // clicked on new object => have to process it
    updateColorWithBuffer( prevActiveId, ColorBufferActionType::RestoreColor );
    if( activeId != CObjectIdGenerator::GetEmptyId() ) {

        updateColorWithBuffer( activeId, ColorBufferActionType::SetColor );

        //auto scripts = stage.GetObjectById( activeId )->GetScripts( EventType::EventClick );
        auto scripts = stage.getScripts( activeId, EventType::EventClick );
        if( !scripts.empty() ) {
            scriptEngine.RunScripts( activeId, EventType::EventClick, scripts );
        }
    }

    RECT rect;
    ::GetClientRect( handle, &rect );
    ::InvalidateRect( handle, &rect, false );
}

void CViewerWindow::onMouseWheel( WPARAM wParam )
{
    scaling( static_cast<signed short>(HIWORD( wParam )) / WHEEL_DELTA );
    Redraw();
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