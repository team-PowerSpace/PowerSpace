#pragma once
#include <Windows.h>

class CScriptEditor
{
public:
	CScriptEditor();
	~CScriptEditor();
	static bool RegisterClass();

	bool Create();

	void Show( int cmdShow );
	std::wstring GetText() const;
	HWND GetHandle() const;
protected:
	void OnNCCreate( HWND hwnd );
	void OnCreate( HWND hwnd );
	void OnDestroy();
	void OnClose();
	void OnCommand( WPARAM wParam );

	void OnFileSave();
	void OnFileOpen();
	void OnFileNew();
private:
	HWND handle;
	HWND editBox; //fix after resolving the problem with creating edit control
	void addToolbar();
	static LRESULT __stdcall windowProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
};