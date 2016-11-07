#pragma once
#include <Windows.h>

class CScriptEditor {
public:
	CScriptEditor();
	~CScriptEditor();
	static bool RegisterClass();

	bool Create();

	void Show( int cmdShow );

	HWND GetHandle();
protected:
	void OnNCCreate( HWND hwnd );
	void OnCreate();
	void OnDestroy();
	void OnClose();
private:
	HWND handle;
	HWND editControl;
	static LRESULT __stdcall windowProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
};