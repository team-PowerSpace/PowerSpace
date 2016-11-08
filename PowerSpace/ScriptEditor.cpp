#include "stdafx.h"
#include "ScriptEditor.h"
#include "Resource.h"
#include <Commctrl.h>
#include <fstream>

#define startCode L"def OnClick(): \r\n\t#put your code here \r\ndef OnTick():\r\n\t#put your code here"

LRESULT __stdcall CScriptEditor::windowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{	
	CScriptEditor* window = 0;
	if( msg == WM_NCCREATE ) {
		window = reinterpret_cast< CScriptEditor*>(	reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		assert( window );
		if( window ) {
			SetWindowLongPtr( handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window) );
			window->OnNCCreate( handle );
		}
		return DefWindowProc( handle, msg, wParam, lParam );
	} else {
		window = reinterpret_cast<CScriptEditor*>(GetWindowLongPtr( handle, GWLP_USERDATA ));
		switch( msg ) {
			case WM_CREATE:
				assert( window != 0 );
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
	editBox = CreateWindowEx(0, L"EDIT", NULL,
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
	wchar_t fileName[MAX_PATH];
	*fileName = 0;
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof( fileName );
	ofn.lpstrFilter = L"Python script(*.py)\0*.py\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = L"Save as";
	ofn.lpstrInitialDir = L"c:\\";
	ofn.lpstrDefExt = ofn.lpstrFilter;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	int result = GetSaveFileName( &ofn );

	if( result ) {
		std::wofstream file( ofn.lpstrFile );				
		file << GetText();
		file.close();		
	}
}

void CScriptEditor::OnFileNew()
{
	SetWindowText( editBox, L" " );
}

void CScriptEditor::OnFileOpen()
{
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;
	memset( &ofn, 0, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof( fileName );
	ofn.lpstrFilter = L"Python script(*.py)\0*.py\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = L"Open";
	ofn.lpstrInitialDir = L"c:\\";
	ofn.lpstrDefExt = ofn.lpstrFilter;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	int result = GetOpenFileName( &ofn );
	if( result ) {
		std::wifstream file;		
		file.open( ofn.lpstrFile );			
		
		std::wstring buffer;
		wchar_t a[50];
		do {
			file.getline(a, 50);
			buffer.append( a );
			buffer.append( L"\r\n" );
		} while( !file.eof() );
		SetWindowText( editBox, buffer.c_str() );
		file.close();
	}

}

std::wstring CScriptEditor::GetText() const
{
	int length = static_cast<int>(SendMessage( editBox, WM_GETTEXTLENGTH, 0, 0 ));
	length++;
	std::wstring text;
	text.resize( length );
	GetWindowText( editBox, (LPWSTR)text.c_str(), length );
	return text;
}