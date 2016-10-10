#pragma once
#include <Windows.h>
#include <string>
#include "EditorRenderingWindow.h"
#include "EditControlWindow.h"
#include "EditorWindow.h"

class CEditor {
public:
	CEditor();
	~CEditor();

	static bool RegisterClass();

	bool Create();
	void Show( int cmdShow );


	std::shared_ptr<CStage>& GetStage();

	void SetStage( std::shared_ptr<CStage> stage_ );

protected:
	// WM_DESTROY
	void OnDestroy();
	//WM_NCCREATE
	void OnNCCreate( HWND handle );
	//WM_CREATE
	void OnCreate();
	// WM_COMMAND, lParam=0
	void OnCommandMenu( WPARAM wParam, LPARAM lParam );
	void OnCommand( WPARAM wParam, LPARAM lParam );
	void OnSize();
	void GetText();

private:
	HWND handle;
	HMENU menu;
	CEditorWindow renderingWindow;
	CEditControlWindow editControl;
	TBox generateDefaultBox() const;
	int searchEmptyId() const;
	HWND saveTextButton;
	HWND setColorButton;
	HWND addScriptButton;

	std::shared_ptr<CStage> stage;

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

	static const int defaultBoxMarginDividor;
};