#pragma once
#include "stdafx.h"
#include "..\Common\Stage.h"


class CRenderingWindow {
public:
	CRenderingWindow();
	virtual ~CRenderingWindow();

	static bool RegisterClass();

	bool Create();
	void Show() const;

	HWND GetHandle() const;

	HWND GetDialog() const;

	CStage& GetStage() const;

	void SetObjects( std::vector<std::shared_ptr<IDrawable>>& );

protected:
	void OnDestroy();

private:
	HWND handle;
	HWND dialog;
	HBITMAP bitmap;
	HDC bitmapContext;
	HBRUSH backgroundBrush;

	float opacity;
	float scaleX;
	float scaleY;
	float centerX;
	float centerY;

	int bitmapWidth;
	int bitmapHeight;

	static const wchar_t* ClassName;
	static const int DefaultWidth;
	static const int DefaultHeight;
	static const COLORREF BackgroundColor;

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

	void onPaint() const;

	void onMouseMove( const WPARAM wParam, const LPARAM lParam );

	void onResize( const RECT* area );

	void drawContent( HDC paintDC, const int width, const int height ) const;

	void drawEraseRectangle( HDC paintDC, const int width, const int height ) const;

	void destroyDoubleBuffer();

};

