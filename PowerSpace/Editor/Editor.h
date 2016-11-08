#pragma once

#pragma comment(lib, "comctl32.lib")

#include "Commctrl.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>

#include "resource.h"

#include "EditorRenderingWindow.h"
#include "EditControlWindow.h"
#include "EditorWindow.h"
#include "EditControlWindow.h"
#include "JsonConverter.h"
#include "ObjectIdGenerator.h"
#include "Stage.h"
#include "StageObjects.h"
#include "Viewer.h"

class CEditor {
public:
	CEditor();
	~CEditor() = default;

	static bool RegisterClass();

	bool Create();
	void Show( int cmdShow );


	std::shared_ptr<CStage>& GetStage();

	void SetStage( std::shared_ptr<CStage> stage_ );

	static CEditor* GetWindowByHandle( HWND handle );

	void SetActiveId( const IdType& id );

protected:
	// WM_DESTROY message handler
	void OnDestroy();
	// WM_NCCREATE message handler
	void OnNCCreate( HWND handle );
	// WM_CREATE message handler
	void OnCreate();
	// WM_COMMAND message handlers
	void OnCommandMenu( WPARAM wParam, LPARAM lParam );
	void OnCommand( WPARAM wParam, LPARAM lParam );

    // WM_RESIZE message handlers
	void OnSize();
	void GetText();

	void OnSave( ) const;

private:
	static const int defaultBoxMarginDividor;

	TBox generateDefaultBox() const;
	void onColorSelect();
	void onFileSelect();

	HBITMAP MakeBitMapTransparent(HBITMAP hbmSrc);
	HBITMAP loadTransparentBitmap(HINSTANCE hInstance, int resource);
	void createToolbar();
	
    static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	
    HWND handle;
	HWND handleToolbar;
	HMENU menu;
	CEditorWindow renderingWindow;
	CEditControlWindow editControl;
	HWND saveTextButton;
	HWND setColorButton;
	HWND addScriptButton;

    COLORREF defaultObjectColor;
	std::shared_ptr<CStage> stage;
	IdType activeId;
};