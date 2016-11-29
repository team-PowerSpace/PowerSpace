#pragma once
#include <ObjectIdGenerator.h>

enum TMarkerType
{
    MT_LeftTop, MT_Top, MT_RightTop, MT_Right, MT_RightBottom, MT_Bottom, MT_LeftBottom, MT_Left, MT_Rotate
};

class Marker
{
public:
    Marker( const RECT & location, const TMarkerType type, const int id );
    const TMarkerType& GetType() const;
    const RECT& GetLocation() const;

    // Id indenity object on stage whitch this marker is related to
    const int GetIndex() const;

    // Returns id of cursor for SetSystemCursor 
    LPCTSTR GetCursor() const;

private:
    TMarkerType type;
    RECT location;
    int index;
};

enum TMovingState
{
    MS_None, MS_Moving, MS_MovingCanvas, MS_Sizing, MS_Rotating
};

class CEditorRenderingWindow
{
public:
    CEditorRenderingWindow();
    virtual ~CEditorRenderingWindow() {};

    bool Create( HWND hWndParent, const wchar_t* classname );
    void Show( int cmdShow ) const;
    HWND GetHandle() const;
    void Redraw() const;

protected:
    void OnDestroy();

    virtual void DrawContent( HDC paintDC, const int width, const int height ) = 0;
    // This metod should be calles in heir-class to draw frame and sizeable marker.
    // Rectangle is stored in this class for moving and sizing.
    // Drawing of object (not frame) should be in heir-class
    void DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const IdType& id, const double angle );

    virtual void MoveCanvas( const POINT& point ) = 0;
    virtual void MoveDrawableObject( const IdType& IdType, const RECT& newSize ) = 0;

    virtual void Scaling( const int direction ) = 0;

    virtual void SelectDrawableObject( const IdType& IdType ) = 0;

    virtual void RotateDrawableObject( const IdType& id, const double newAngle ) = 0;

    static LRESULT __stdcall WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
    static INT_PTR CALLBACK dialogSetText( HWND hwnd, UINT msg, WPARAM wParam );

private:
    static const int defaultWidth;
    static const int defaultHeight;
    static const int markerHalfSize;
    static const int rotateMarkerShift;
    static const COLORREF backgroundColor;
    static const COLORREF markerColor;
    static const COLORREF borderColor;

    HWND handle;
    HBITMAP bitmap;
    HDC bitmapContext;
    HBRUSH backgroundBrush;
    HPEN borderPen;
    HBRUSH markerBrush;
    HPEN markerPen;

    int bitmapWidth;
    int bitmapHeight;

    std::vector<Marker> markers;
    std::vector<RECT> rectangles;
    std::vector<IdType> rectanglesIds;

    POINT canvasPoint;
    TMovingState currentMovingState;
    // below fields not required to be intialized
    POINT lastPoint;
    RECT startSize;
    RECT lastSize;
    TMarkerType sizingMarkerType;
    IdType selectedId;

    double startAngle;
    double lastAngle;
    POINT rectCentre;
    std::vector<double> angles;

    void onPaint();
    void onMouseMove( const WPARAM wParam, const LPARAM lParam );
    void onMouseMove( const LPARAM lParam );
    void onMouseWheel( WPARAM wParam );
    void onMouseDown( const LPARAM lparam );
    void onMouseUpOrLeave( const LPARAM lparam );
    void onResize( const RECT* area );
    void onDoubleClick( const LPARAM lParam );

    void drawEraseRectangle( HDC paintDC, const int width, const int height ) const;

    void addMarkersForRectangle( HDC paintDC, const int x, const int y, const int width, const int height, const IdType& id, const int index );
    void addMarker( HDC paintDC, const int x, const int y, const TMarkerType type, const IdType& id, const int index );

    void destroyDoubleBuffer();

    LPCTSTR getCursor( const POINT& point ) const;

    RECT resizeRect( const POINT& point );

    static bool contains( const RECT& rect, const POINT& point );

    static POINT getPointByLParam( const LPARAM& lparam );

    double getAngleBetweenPoints( const POINT& point_start, const POINT& point_end, const POINT& point_centre ) const;
    POINT getRectangleCentre( const RECT& rect ) const;
};
