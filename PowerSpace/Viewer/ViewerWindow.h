#pragma once
#include "Stage.h"
#include "Resource.h"
#include "ScriptEngine.h"

// win32 window of the viewer module
// has references to viewport and canvas (to modify them easily)
class CViewerWindow {
public:
	CViewerWindow( CStage& stage, CViewport& _viewport, CCanvas& _canvas );
	~CViewerWindow();

	// all the WinAPI stuff
	static bool RegisterClass();

	bool Create();

	void Show() const;

	HWND GetHandle() const;

protected:

	void onClose();

	static LRESULT __stdcall WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	
private:
	HWND handle;
	HBITMAP bitmap;
	HDC bitmapContext;
	HBRUSH backgroundBrush;
	HBRUSH markerBrush;
	HPEN markerPen;

	int bitmapWidth;
	int bitmapHeight;

	const CViewport& viewport;
	const CCanvas& canvas;
    CStage& stage;
    CScriptEngine scriptEngine;

	const int windowHeight;
	const int windowWidth;

	int activeId;

	static const wchar_t* ClassName;
	static const wchar_t* ViewerApplicationName;

	POINT getMouseCoords( LPARAM lParam );

	bool pointInBox( TBox box, POINT point );

	void onCreate();

	void onTimer();

	void onPaint();

	void onSize();

	void onMouseMove( const WPARAM wParam, const LPARAM lParam );

	void onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam );
};