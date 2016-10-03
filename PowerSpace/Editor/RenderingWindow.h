#pragma once


enum MarkerType {
	LeftTop, Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left
};

class Marker {
public:
	Marker( const RECT & location, const MarkerType type, const int id );
	const MarkerType& GetType() const;
	const RECT& GetLocation() const;

	// Id indenity object on stage whitch this marker is related to
	const int GetId() const;

	// Returns id of cursor for SetSystemCursor 
	DWORD GetCursor() const;

private:
	MarkerType type;
	RECT location;
	int id;
};

class CRenderingWindow {
public:
	CRenderingWindow();
	virtual ~CRenderingWindow();

	void Show() const;

	HWND GetHandle() const;

protected:
	bool Create( const wchar_t* classname );

	void OnDestroy();

	virtual void drawContent( HDC paintDC, const int width, const int height ) = 0;

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

	// This metod should be calles in heir-class to draw frame and sizeable marker.
	// Rectangle is stored in this class for moving and sizing.
	// Drawing of object (not frame) should be in heir-class
	void DrawSizeableRectangle( HDC paintDC, const RECT & rectangle, const int id );

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


	void onPaint();

	void onMouseMove( const WPARAM wParam, const LPARAM lParam );

	void onResize( const RECT* area );

	void drawEraseRectangle( HDC paintDC, const int width, const int height ) const;


	void AddMarkersForRectangle( HDC paintDC, const int x, const int y, const int width, const int height, const int id );

	void AddMarker( HDC paintDC, const int x, const int y, const MarkerType type, const int id );

	void destroyDoubleBuffer();

};
