#include <Windows.h>
#include <iostream>
#include "stdafx.h"
#include "Editor.h"
#include "resource.h"
#include "EditControlWindow.h"

#define IDC_MAIN_BUTTON 101 

CEditor::CEditor()
{
	menu = 0;
	//renderingWindow = CEditorRenderingWindow();
	ellipseWindow = CEllipseWindow();
	editControl = CEditControlWindow();
	saveTextButton = {};
}

CEditor::~CEditor()
{
}

bool CEditor::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory( &windowClass, sizeof( windowClass ) );
	windowClass.cbSize = sizeof( WNDCLASSEX );
	windowClass.lpfnWndProc = CEditor::windowProc;
	windowClass.hInstance = GetModuleHandle( 0 );
	windowClass.lpszClassName = L"CEditor";
	windowClass.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );
	windowClass.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );
	return (::RegisterClassEx( &windowClass ) != 0);
}

bool CEditor::Create()
{
	CreateWindowEx( 0, L"CEditor", L"My Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle( 0 ), this );
	return (handle != 0);
}

void CEditor::Show( int cmdShow )
{
	ShowWindow( handle, cmdShow );
	//renderingWindow.Show();
	ellipseWindow.Show( cmdShow );
	ShowWindow( saveTextButton, cmdShow );
}

void CEditor::OnDestroy()
{
	DestroyMenu(menu);
	PostQuitMessage(0);
}

void CEditor::OnNCCreate( HWND _handle ) {
	handle = _handle;
}

void CEditor::OnCreate()
{
	editControl.Create(handle);
	//CEditorRenderingWindow::RegisterClass();
	CEllipseWindow::RegisterClass();
	menu = LoadMenu( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDR_MENU1 ) );
	//renderingWindow.Create( L"CEditor" );
	ellipseWindow.Create(handle);
	saveTextButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Save Text",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		100,        // Button width
		100,        // Button height
		handle,     // Parent window
		(HMENU)IDC_MAIN_BUTTON,       // menu.
		(HINSTANCE)GetWindowLong(handle, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

}


void CEditor::OnSize()
{
	RECT rect;
	int middleX, nWidth, nHeight;
	::GetClientRect(handle, &rect);
	middleX = ( rect.left + rect.right ) / 4;
	nWidth = ( rect.right - rect.left ) * 3 / 4;
	nHeight = ( rect.bottom - rect.top );
	//SetWindowPos( renderingWindow.GetHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0 );
	SetWindowPos(ellipseWindow.getHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0);
	SetWindowPos(editControl.GetHandle(), HWND_TOP, rect.left, rect.top, nWidth / 3, 
		nHeight * 3 / 4, 0);
	SetWindowPos(saveTextButton, HWND_TOP, rect.left, rect.top + nHeight * 3 / 4, nWidth / 3,
		nHeight / 4, 0);
}

void CEditor::GetText()
{
	int length = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t *text = new wchar_t[length + 1];
	SendMessage(editControl.GetHandle(), WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text));
	//add functions here
	delete[] text;
}

void CEditor::OnCommandMenu( WPARAM wParam, LPARAM lParam )
{
	switch LOWORD( wParam )
	{
	case ID_ADD_RECTANGLE:
	{
		//add rectangle
		break;
	}
	case ID_ADD_ELLIPSE:
	{
		//add ellipse
		break;
	}
	case ID_PLAY_LAUNCHPLAYER:
	{
		//launch player
		break;
	}
	case IDC_MAIN_BUTTON:
	{
		GetText();
		break;
	}	
	}
}

void CEditor::OnCommand( WPARAM wParam, LPARAM lParam )
{
	//WORD x =HIWORD( wParam );
	switch ( HIWORD( wParam ) )
	{
	case 0:
	{
		OnCommandMenu( wParam, lParam );
		break;
	}
	}
}

LRESULT CEditor::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message ) {
	case WM_NCCREATE:
	{
		CEditor* window = (CEditor*)( (CREATESTRUCT*)lParam )->lpCreateParams;
		SetLastError( 0 );
		SetWindowLongPtr( handle, GWLP_USERDATA, (LONG)window );
		if ( GetLastError() != 0 ) {
			return GetLastError();
		}
		window->OnNCCreate( handle );
		return DefWindowProc( handle, message, wParam, lParam );
	}
	case WM_CREATE:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr( handle, GWLP_USERDATA );
		window->OnCreate();
		return DefWindowProc( handle, message, wParam, lParam );
	}
	case WM_SIZE:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr( handle, GWLP_USERDATA );
		window->OnSize();
		return DefWindowProc( handle, message, wParam, lParam );
	}
	case WM_COMMAND:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr( handle, GWLP_USERDATA );
		window->OnCommand( wParam, lParam );
		return DefWindowProc( handle, message, wParam, lParam );
	}
	case WM_DESTROY:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr( handle, GWLP_USERDATA );
		window->OnDestroy();
		return 0;
	}
	default:
		return DefWindowProc( handle, message, wParam, lParam );
	}
}
