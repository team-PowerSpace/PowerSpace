#pragma once
#include "stdafx.h"

class CViewerRenderingWindow
{
public:
	CViewerRenderingWindow();
	virtual ~CViewerRenderingWindow();

	void Show() const;

	HWND GetHandle() const;

protected:
	bool Create( const wchar_t* classname );

	void OnDestroy();

	virtual void DrawContent( HDC PaintDC, const int width, const int height ) = 0;

	static LRESULT __stdcall WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	HWND handle;

	const int windowHeight;
	const int windowWidth;
};