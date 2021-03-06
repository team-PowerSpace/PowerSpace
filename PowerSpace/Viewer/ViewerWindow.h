#pragma once
#include <Stage.h>
#include "resource.h"
#include <ScriptEngine.h>

enum ColorBufferActionType { RestoreColor, SetColor };

enum TMovingState_Viewer
{
    MSV_None, MSV_MovingCanvas
};

// win32 window of the viewer module
// has references to viewport and canvas (to modify them easily)
class CViewerWindow
{
public:
    CViewerWindow( CStage& stage, CCanvas& _canvas );
    ~CViewerWindow();

    // all the WinAPI stuff
    static bool RegisterClass();

    bool Create();

    void Show() const;

    void Redraw() const;

    HWND GetHandle() const;

private:
    HWND handle;
    HBITMAP bitmap;
    HDC bitmapContext;
    HBRUSH backgroundBrush;
    HACCEL haccel;

    CViewport& viewport;
    const CCanvas& canvas;
    CStage& stage;
    CScriptEngine scriptEngine;

	static const wchar_t* ClassName; // win32 features
	static const wchar_t* ViewerApplicationName;
	static const COLORREF backgroundColor;

    const double scalingFactor = 1.2;

    const int windowHeight; // window params
    const int windowWidth;
    int bitmapWidth; // canvas params
    int bitmapHeight;
    TMovingState_Viewer currentMovingState; // for drag'n'drop on canvas
    POINT prevPoint, canvasPoint;

    IdType activeId; // id of currently active object
    int colorBuf; // Bufferized color of object for clever selection

    bool viewerIsRunning; // Pause/Play indicator

    void scaling( const int direction );

    static LRESULT __stdcall WindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

    void onCreate();

    void onClose();

    void onTimer();

    void onPaint();

    void onSize();

    void onMouseMove( const WPARAM wParam, const LPARAM lParam );

    void onMouseClick( UINT msg, const WPARAM wParam, const LPARAM lParam );

    void onMouseWheel( WPARAM wParam );

    void onCommand( WPARAM wParam, LPARAM lParam );

    void enableTimer( int timeDelay, int timerId = 0 );

    void disableTimer( int timerId = 0 );

    POINT getMouseCoords( LPARAM lParam );

    bool isPointInBox( TBox box, POINT point );

	void updateColorWithBuffer( IdType prevActiveId, ColorBufferActionType actionType );

	void redraw() const;

	void fillBackground( HDC paintDC, const int width, const int height ) const;

	void rotateWorld( HDC thisBitmapContext );
};