#pragma once
#include <Windows.h>
#include <string>
#include "EditorRenderingWindow.h"
#include "EllipseWindow.h"
#include "EditControlWindow.h"

class CEditor {
public:
	CEditor();
	~CEditor();

	static bool RegisterClass();

	bool Create();
	void Show( int cmdShow );

protected:
	// WM_DESTROY
	void OnDestroy();
	//WM_NCCREATE
	void OnNCCreate( HWND handle );
	//WM_CREATE
	void OnCreate();
	// WM_COMMAND, lParam=0
	void OnCommandMenu( WPARAM wParam, LPARAM lParam);
	void OnCommand( WPARAM wParam, LPARAM lParam );
	void OnSize();
	void GetText();

private:
	HWND handle;
	HMENU menu;
	CEditorRenderingWindow renderingWindow;
	CEditControlWindow editControl;
	HWND saveTextButton;

	CEllipseWindow ellipseWindow;

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};