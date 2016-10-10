#pragma once

enum TMarkerType {
	MT_LeftTop, MT_Top, MT_RightTop, MT_Right, MT_RightBottom, MT_Bottom, MT_LeftBottom, MT_Left
};

class Marker {
public:
	Marker( const RECT & location, const TMarkerType type, const int id );
	const TMarkerType& GetType() const;
	const RECT& GetLocation() const;

	// Id indenity object on stage whitch this marker is related to
	const int GetId() const;

	// Returns id of cursor for SetSystemCursor 
	LPCTSTR GetCursor() const;

private:
	TMarkerType type;
	RECT location;
	int id;
};

enum TMovingState {
	MS_None, MS_Moving, MS_MovingCanvas, MS_Sizing
};

class CEditorRenderingWindow {
public:
	CEditorRenderingWindow();
	virtual ~CEditorRenderingWindow();

	void Show(int cmdShow) const;

	HWND GetHandle() const;

protected:

	bool Create(HWND hWndParent, const wchar_t* classname);

	void OnDestroy();

	virtual void DrawContent(HDC paintDC, const int width, const int height) {};

	static LRESULT __stdcall WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

	// This metod should be calles in heir-class to draw frame and sizeable marker.
	// Rectangle is stored in this class for moving and sizing.
	// Drawing of object (not frame) should be in heir-class
	void DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const int id );

	virtual void MoveCanvas( const POINT& point ) = 0;

	virtual void MoveRectangle( const int id, const RECT& newSize) = 0;

	virtual void Scaling( const int direction ) = 0;

	virtual void SelectRectangle( const int id ) = 0;

private:
	HWND handle;
	HWND dialog;
	HBITMAP bitmap;
	HDC bitmapContext;
	HBRUSH backgroundBrush;
	HBRUSH markerBrush;
	HPEN markerPen;

	int bitmapWidth;
	int bitmapHeight;

	std::vector<Marker> markers;
	std::vector<RECT> rectangles;
	std::vector<int> rectanglesIds;


	static const int DefaultWidth;
	static const int DefaultHeight;
	static const int MarkerHalfSize;
	static const COLORREF BackgroundColor;
	static const COLORREF MarkerColor;

	POINT canvasPoint;
	TMovingState currentMovingState;
	// below fields not required to be intialized
	POINT lastPoint;
	RECT startSize;
	RECT lastSize;
	TMarkerType sizingMarkerType;
	int selectedId;


	void onPaint();

	void onMouseMove( const WPARAM wParam, const LPARAM lParam );

	void onMouseMove( const LPARAM lParam );

	void onResize( const RECT* area );

	void drawEraseRectangle( HDC paintDC, const int width, const int height ) const;


	void addMarkersForRectangle( HDC paintDC, const int x, const int y, const int width, const int height, const int id );

	void addMarker( HDC paintDC, const int x, const int y, const TMarkerType type, const int id );

	void destroyDoubleBuffer();

	LPCTSTR getCursor( const POINT& point ) const;

	void reDraw() const;

	RECT resizeRect( const POINT& point );

	void onMouseWheel( WPARAM wParam );

	static bool contains( const RECT& rect, const POINT& point );
	
	void onMouseDown( const LPARAM lparam );

	void onMouseUpOrLeave( const LPARAM lparam );

	static POINT getPointByLParam( const LPARAM& lparam );
};
