#include <stdafx.h>
#include <iostream>
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
	handle = CreateWindowEx( 0, L"CEditor", L"My Window", WS_OVERLAPPEDWINDOW,
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
		*(HINSTANCE*)GetWindowLongPtr( handle, GWLP_HINSTANCE ), NULL );
	setColorButton = CreateWindow( L"BUTTON", L"Set Color", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
        *(HINSTANCE*)GetWindowLongPtr( handle, GWLP_HINSTANCE ), NULL );
	addScriptButton = CreateWindow( L"BUTTON", L"Add Script", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
        *(HINSTANCE*)GetWindowLongPtr( handle, GWLP_HINSTANCE ), NULL );

	EnableWindow( editControl.GetHandle(), false );
	EnableWindow( saveTextButton, false );
	EnableWindow( setColorButton, false );
	EnableWindow( addScriptButton, false );

}

CEditor* CEditor::GetWindowByHandle( HWND handle )
{
	return reinterpret_cast<CEditor*>(GetWindowLongPtr( handle, GWLP_USERDATA ));
}

void  CEditor::SetActiveId( const int id )
{
	activeId = id;
	if( id < 0 ) {
		EnableWindow( setColorButton, false );
		EnableWindow( addScriptButton, false );
	} else {
		EnableWindow( setColorButton, true );
		EnableWindow( addScriptButton, true );
	}
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
	int length = static_cast<int>(SendMessage( editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0 ));
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
			CViewer viewer( *stage, CViewport() );
            if( viewer.Create() ) {
                const CViewerWindow& viewerWindow = viewer.GetViewerWindow();
                HWND viewerHandle = viewerWindow.GetHandle();;
                MSG message;
                BOOL getMessageResult = 0;
                while( (getMessageResult = ::GetMessage( &message, viewerHandle, 0, 0 )) != 0 ) {
                    if( getMessageResult == -1 ) {
                        break;
                    }
                    ::TranslateMessage( &message );
                    ::DispatchMessage( &message );
                }
            }
			break;
		}
        case IDC_MAIN_BUTTON:
        {
            if( (HWND)lParam == saveTextButton ) {
                GetText();
            } else if( (HWND)lParam == setColorButton ) {
                onColorSelect();
            } else if( (HWND)lParam == addScriptButton ) {
                onFileSelect();
            }
            break;
        }
	}
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

void CEditor::onColorSelect()
{
	CHOOSECOLOR chooseColor;
	static COLORREF customColors[16];
	ZeroMemory( &chooseColor, sizeof( chooseColor ) );
	chooseColor.lStructSize = sizeof( chooseColor );
	chooseColor.hwndOwner = handle;
	chooseColor.lpCustColors = reinterpret_cast<LPDWORD>(customColors);
	chooseColor.Flags = CC_FULLOPEN | CC_RGBINIT;
	chooseColor.rgbResult = stage->GetObjectById( activeId )->GetColor();
	if( ::ChooseColor( &chooseColor ) ) {
		stage->GetObjectById( activeId )->SetColor( chooseColor.rgbResult );
		renderingWindow.ReDraw();
	}
}

void CEditor::onFileSelect()
{
	OPENFILENAMEW filename;
	filename.lStructSize = sizeof( filename );
	filename.hwndOwner = handle;
	filename.hInstance = GetModuleHandleW( nullptr );
	filename.lpstrFilter = L"Python script (*.py)\0*.py\0All Files (*.*)\0*.*\0\0";
	filename.lpstrCustomFilter = 0;
	filename.nFilterIndex = 1;
	filename.nMaxFile = 1024;
	std::unique_ptr<wchar_t[]> name( new wchar_t[filename.nMaxFile] );
	filename.lpstrFile = name.get();
	filename.lpstrFile[0] = 0;
	filename.lpstrInitialDir = 0;
	filename.lpstrTitle = L"Add script";
	filename.nMaxFileTitle = 0;
	filename.lpstrDefExt = L".py";
	filename.lCustData = 0;
	filename.lpfnHook = 0;
	filename.lpTemplateName = 0;
	filename.nFileOffset = 0;
	filename.nFileExtension = 0;
	filename.Flags = OFN_FILEMUSTEXIST;

	if( ::GetOpenFileName( &filename ) ) {
		CScript script( filename.lpstrFile ); //Add choice of event type
		stage->addScript(EventType::EventClick, activeId, script);
        //stage->GetObjectById( activeId )->AddScript( EventType::EventClick, script );
	}
}

LRESULT CEditor::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message ) {
		case WM_NCCREATE:
		{
			CEditor* window = (CEditor*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetLastError( 0 );
			SetWindowLongPtr( handle, GWLP_USERDATA, (LONG_PTR)window );
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
