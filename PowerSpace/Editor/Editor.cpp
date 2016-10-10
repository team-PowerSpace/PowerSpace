#include <Windows.h>
#include <iostream>
#include "stdafx.h"
#include "Editor.h"
#include "resource.h"
#include "EditControlWindow.h"
#include "Viewer.h"
#include "Stage.h"
#include "StageObjects.h"

#define IDC_MAIN_BUTTON 101 

CEditor::CEditor()
{
	menu = 0;
	renderingWindow = CEditorWindow();
	editControl = CEditControlWindow();
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
	renderingWindow.Show( cmdShow );
	ShowWindow( saveTextButton, cmdShow );
	ShowWindow( setColorButton, cmdShow );
	ShowWindow( addScriptButton, cmdShow );
}

std::shared_ptr<CStage>& CEditor::GetStage()
{
	return stage;
}

void CEditor::SetStage( std::shared_ptr<CStage> stage_ )
{
	stage = stage_;
	renderingWindow.SetStage( stage_ );
}

void CEditor::OnDestroy()
{
	DestroyMenu( menu );
	PostQuitMessage( 0 );
}

void CEditor::OnNCCreate( HWND _handle )
{
	handle = _handle;
}

void CEditor::OnCreate()
{
	editControl.Create( handle );
	CEditorWindow::RegisterClass();
	menu = LoadMenu( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDR_MENU1 ) );
	renderingWindow.Create( handle );
	saveTextButton = CreateWindow( L"BUTTON", L"Save Text", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
		(HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE ), NULL );
	setColorButton = CreateWindow( L"BUTTON", L"Set Color", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
		(HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE ), NULL );
	addScriptButton = CreateWindow( L"BUTTON", L"Add Scrypt", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
		(HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE ), NULL );

}


void CEditor::OnSize()
{
	RECT rect;
	int middleX, nWidth, nHeight;
	::GetClientRect( handle, &rect );
	middleX = (rect.left + rect.right) / 4;
	nWidth = (rect.right - rect.left) * 3 / 4;
	nHeight = (rect.bottom - rect.top);
	SetWindowPos( renderingWindow.GetHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0 );
	SetWindowPos( editControl.GetHandle(), HWND_TOP, rect.left, rect.top, nWidth / 3,
		nHeight * 3 / 4, 0 );
	SetWindowPos( saveTextButton, HWND_TOP, rect.left, rect.top + nHeight * 3 / 4,
		nWidth / 3, nHeight / 12, 0 );
	SetWindowPos( setColorButton, HWND_TOP, rect.left, rect.top + nHeight * 3 / 4 + nHeight / 12,
		nWidth / 3, nHeight / 12, 0 );
	SetWindowPos( addScriptButton, HWND_TOP, rect.left, rect.top + nHeight * 3 / 4 + nHeight / 6,
		nWidth / 3, nHeight / 12, 0 );
}

void CEditor::GetText()
{
	int length = SendMessage( editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0 );
	wchar_t *text = new wchar_t[length + 1];
	SendMessage( editControl.GetHandle(), WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text) );
	//add functions here
	delete[] text;
}

void CEditor::OnCommandMenu( WPARAM wParam, LPARAM lParam )
{
	switch LOWORD( wParam )
	{
		case ID_ADD_RECTANGLE:
		{
			// TODO const for color
			stage->GetObjects().insert( std::pair<int, std::shared_ptr<IDrawable>>( searchEmptyId(),
				std::make_shared<CRectangleObject>( RGB( 100, 90, 80 ), generateDefaultBox() ) ) );
			renderingWindow.ReDraw();
			break;
		}
		case ID_ADD_ELLIPSE:
		{
			// TODO const for color
			stage->GetObjects().insert( std::pair<int, std::shared_ptr<IDrawable>>( searchEmptyId(),
				std::make_shared<CEllipseObject>( RGB( 100, 90, 80 ), generateDefaultBox() ) ) );
			renderingWindow.ReDraw();
			break;
		}
		case ID_PLAY_LAUNCHPLAYER:
		{
			CViewer viewer( *stage );
			break;
		}
		case IDC_MAIN_BUTTON:
		{
			{
				if( (HWND)lParam == saveTextButton ) {
					GetText();
				} else if( (HWND)lParam == setColorButton ) {
					//set color
				} else if( (HWND)lParam == addScriptButton ) {
					//add Script
				}
				break;
			}
		}
	}
	UNREFERENCED_PARAMETER( lParam );
}

void CEditor::OnCommand( WPARAM wParam, LPARAM lParam )
{
	switch( HIWORD( wParam ) ) {
		case 0:
		{
			OnCommandMenu( wParam, lParam );
			break;
		}
	}
}

TBox CEditor::generateDefaultBox() const
{
	TBox box;
	RECT rect;
	GetWindowRect( renderingWindow.GetHandle(), &rect );
	long width = rect.right - rect.left;
	long height = rect.bottom - rect.top;
	box.left = width / defaultBoxMarginDividor;
	box.top = height / defaultBoxMarginDividor;
	box.right = 3 * width / defaultBoxMarginDividor;
	box.bottom = 3 * height / defaultBoxMarginDividor;
	return stage->GetViewPort().ConvertToModelCoordinates( box );
}

int CEditor::searchEmptyId() const
{
	int maxid = 0;
	for( auto i : stage->GetObjects() ) {
		if( i.first > maxid ) {
			maxid = i.first;
		}
	}
	return maxid + 1;
}

LRESULT CEditor::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message ) {
		case WM_NCCREATE:
		{
			CEditor* window = (CEditor*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetLastError( 0 );
			SetWindowLongPtr( handle, GWLP_USERDATA, (LONG)window );
			if( GetLastError() != 0 ) {
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

const int CEditor::defaultBoxMarginDividor = 4;