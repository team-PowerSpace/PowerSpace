#include "stdafx.h"
#include "ScriptEditor.h"
#include "Resource.h"
#include <Commctrl.h>

LPWSTR startCode = L"def OnClick(): \r\n\t#put your code here \r\ndef OnTick():\r\n\t#put your code here";

LRESULT __stdcall CScriptEditor::windowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{	
	CScriptEditor* window;
	if( msg == WM_NCCREATE ) {
		window = reinterpret_cast< CScriptEditor*>(	reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		if( window ) {
			SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(window) );
			window->OnNCCreate( handle );
			return 1;
		}
		return 0;
	} else {
		window = reinterpret_cast<CScriptEditor*>(GetWindowLongPtr( handle, 0 ));
		switch( msg ) {
			case WM_CREATE:
				window->OnCreate( handle );
				return 1;
			case WM_CLOSE:
				window->OnClose();
				return 0;
			case WM_COMMAND:
				window->OnCommand( wParam);
				return 0;
			default:
				return DefWindowProc( handle, msg, wParam, lParam );
		}
	}
}

bool CScriptEditor::RegisterClass()
{
	HMODULE instance = GetModuleHandleW( nullptr );
	WNDCLASSEX windowClass;
	ZeroMemory( &windowClass, sizeof( windowClass ) );
	windowClass.cbSize = sizeof( WNDCLASSEX );
	windowClass.lpfnWndProc = windowProc;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"ScriptEditor";
	return (RegisterClassEx( &windowClass ) != 0);
}

CScriptEditor::CScriptEditor() {
	
}

CScriptEditor::~CScriptEditor()
{
	if( handle ) {
		DestroyWindow( handle );
	}
}

void CScriptEditor::OnNCCreate( HWND _handle )
{
	handle = _handle;
}

bool CScriptEditor::Create()
{
	HINSTANCE hInstance = GetModuleHandle( nullptr );
	
	handle = CreateWindowEx( 0, L"ScriptEditor", L"ScriptEditor",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
		hInstance, this );	
	addToolbar();
	return (handle != 0);
}

void CScriptEditor::addToolbar()
{
	HWND toolbar = CreateToolbarEx( handle, 0, 0,
		0, HINST_COMMCTRL, NULL, NULL, 0, 0, 0, 0, 0, sizeof( TBBUTTON ) );
	TBBUTTON tbb_buildin[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage( toolbar, (UINT)TB_ADDBUTTONS, _countof( tbb_buildin ), (LPARAM)&tbb_buildin );
	ShowWindow( toolbar, TRUE );
}

HWND CScriptEditor::GetHandle() const
{
	return handle;
}

void CScriptEditor::OnCreate( HWND hwnd )
{
	RECT windowRect;	
	GetClientRect( hwnd, &windowRect );	
	HWND editBox = CreateWindowEx(0, L"EDIT", NULL,
		WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | WS_BORDER,
		windowRect.left, windowRect.top + 30,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		hwnd, NULL, GetModuleHandle( 0 ), NULL );	
	SetWindowText( editBox, startCode );
}

void CScriptEditor::OnClose()
{
	if( handle ) {
		DestroyWindow( handle );
	}
}

void CScriptEditor::OnDestroy()
{
	PostQuitMessage( 0 );
}

void CScriptEditor::Show( int cmdShow )
{
	ShowWindow( handle, cmdShow );
}

void CScriptEditor::OnCommand( WPARAM wParam)
{
	switch( LOWORD( wParam ) ) {
		case ID_FILE_SAVE:
		{
			OnFileSave();
			break;
		}
		case ID_FILE_NEW:
		{
			OnFileNew();
			break;
		}
		case ID_FILE_OPEN:
		{
			OnFileOpen();
			break;
		}
		default:
			break;
	}
}

void CScriptEditor::OnFileSave()
{
	MessageBox( NULL, L"FILE", L"SAVE", NULL );
}

void CScriptEditor::OnFileNew()
{
	MessageBox( NULL, L"FILE", L"NEW", NULL );
}

void CScriptEditor::OnFileOpen()
{
	MessageBox( NULL, L"FILE", L"OPEN", NULL );
}