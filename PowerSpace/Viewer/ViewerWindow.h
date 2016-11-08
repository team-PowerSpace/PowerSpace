#pragma once
#include "Stage.h"
#include "Resource.h"
#include "ScriptEngine.h"

#define BLUE_FOR_CANVAS_CROSS RGB(0, 128, 255)
enum ColorBufferActionType { RestoreColor, SetColor };

// win32 window of the viewer module
// has references to viewport and canvas (to modify them easily)
class CViewerWindow
{
public:
	CViewerWindow( CStage& stage, CViewport& _viewport, CCanvas& _canvas );
	~CViewerWindow();

	// all the WinAPI stuff
	static bool RegisterClass();

	bool Create();

	void Show() const;

	HWND GetHandle() const;

private:
	HWND handle;
	HBITMAP bitmap;
	HDC bitmapContext;
	HBRUSH backgroundBrush;

	const CViewport& viewport;
	const CCanvas& canvas;
	CStage& stage;
	CScriptEngine scriptEngine;

	static const wchar_t* ClassName; // win32 features
	static const wchar_t* ViewerApplicationName;

	const int windowHeight; // window params
	const int windowWidth;
	int bitmapWidth; // canvas params
	int bitmapHeight;

	IdType activeId; // id of currently active object
	int colorBuf; // Bufferized color of object for clever selection

	bool viewerIsRunning; // Pause/Play indicator

	static LRESULT __stdcall WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

	void onCreate();

	void onClose();

	void onTimer();

	void onPaint();

	void onSize();

	void onMouseMove( const WPARAM wParam, const LPARAM lParam );

	void onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam );

	void onCommand( WPARAM wParam, LPARAM lParam );

	void onCommandMenu( WPARAM wParam, LPARAM lParam );

	void enableTimer( int timeDelay, int timerId = 0 );

	void disableTimer( int timerId = 0 );

	POINT getMouseCoords( LPARAM lParam );

	bool isPointInBox( TBox box, POINT point );

	void updateColorWithBuffer( IdType prevActiveId, ColorBufferActionType actionType );
};