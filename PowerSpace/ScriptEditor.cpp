#include "stdafx.h"
#include "ScriptEditor.h"

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
				window->OnCreate();
				return 1;
			case WM_CLOSE:
				window->OnClose();
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
	handle = nullptr;
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
	HINSTANCE hInstance = ::GetModuleHandle( nullptr );

	handle = CreateWindowEx( 0, L"ScriptEditor", L"ScriptEditor",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
		hInstance, this );	
	return (handle != 0);
}

HWND CScriptEditor::GetHandle()
{
	return handle;
}

void CScriptEditor::OnCreate()
{
	
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