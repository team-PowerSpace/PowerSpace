#pragma once
#include <Windows.h>
#include <string>
#include "EditorRenderingWindow.h"

class CEditor {
public:
	CEditor();
	~CEditor();

	static bool RegisterClass();

	bool Create();
	void Show(int cmdShow);

protected:
	// WM_DESTROY
	void OnDestroy();
	//WM_NCCREATE
	void OnNCCreate(HWND handle);
	//WM_CREATE
	void OnCreate();
	// WM_COMMAND, lParam=0
	void OnCommandMenu(WPARAM wParam);
	void OnCommand(WPARAM wParam);
	void OnSize();

private:
	HWND handle;
	HMENU menu;
	CEditorRenderingWindow renderingWindow;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};