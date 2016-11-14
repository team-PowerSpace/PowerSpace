#include <stdafx.h>
#include <iostream>
#include <fstream>
#include "Editor.h"
#include "resource.h"
#include "EditControlWindow.h"
#include "Viewer.h"
#include "Stage.h"
#include "StageObjects.h"
#include "JsonConverter.h"
#include "ScriptEditor.h"
#include <JsonWorker.h>
#define IDC_MAIN_BUTTON 101 

const int CEditor::defaultBoxMarginDividor = 4;

CEditor::CEditor()
    : menu(0), defaultObjectColor( RGB( 100, 90, 80 ) )
{}

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

	createToolbar();

	return (handle != 0);
}

void CEditor::Show( int cmdShow )
{
	ShowWindow( handle, cmdShow );
	renderingWindow.Show( cmdShow );
	ShowWindow( setFontButton, cmdShow);
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
	setFontButton = CreateWindow(L"BUTTON", L"Set Font", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100, handle, (HMENU)IDC_MAIN_BUTTON,
		*(HINSTANCE*)GetWindowLongPtr(handle, GWLP_HINSTANCE), NULL);
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
	EnableWindow( setFontButton, false );
	EnableWindow( saveTextButton, false );
	EnableWindow( setColorButton, false );
	EnableWindow( addScriptButton, false );

}

CEditor* CEditor::GetWindowByHandle( HWND handle )
{
	return reinterpret_cast<CEditor*>(GetWindowLongPtr( handle, GWLP_USERDATA ));
}

void CEditor::SetActiveId( const IdType& id )
{
	activeId = id;
	wchar_t firstLetter = id[0];
	if ( firstLetter ==  L't') {
		EnableWindow( setFontButton, true );
		EnableWindow( setColorButton, true );
		EnableWindow( addScriptButton, true );
	}
	else {
		if (id == CObjectIdGenerator::GetEmptyId()) {
			EnableWindow( setFontButton, false );
			EnableWindow( setColorButton, false );
			EnableWindow( addScriptButton, false );
		}
		else {
			EnableWindow( setFontButton, false );
			EnableWindow( setColorButton, true );
			EnableWindow( addScriptButton, true );
		}
	}
}

HBITMAP CEditor::MakeBitMapTransparent(HBITMAP hbmSrc) {
	HDC hdcSrc, hdcDst;
	BITMAP bm;
	COLORREF clrTP, clrBK;
	HBITMAP hbmOld, hbmNew = NULL;

	if ((hdcSrc = CreateCompatibleDC(NULL)) != NULL) {
		if ((hdcDst = CreateCompatibleDC(NULL)) != NULL) {
			int nRow, nCol;
			GetObject(hbmSrc, sizeof(bm), &bm);
			hbmOld = (HBITMAP)SelectObject(hdcSrc, hbmSrc);
			hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
			SelectObject(hdcDst, hbmNew);

			BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

			clrTP = GetPixel(hdcDst, 1, 1);// Get color of first pixel at 0,0
			clrBK = GetSysColor(COLOR_MENU);// Get the current background color of the menu

			for (nRow = 0; nRow < bm.bmHeight; nRow++) {// work our way through all the pixels changing their color
				for (nCol = 0; nCol < bm.bmWidth; nCol++) {// when we hit our set transparency color.
					if (GetPixel(hdcDst, nCol, nRow) == clrTP) {
						SetPixel(hdcDst, nCol, nRow, clrBK);
					}
				}
			}
			DeleteDC(hdcDst);
		}
		DeleteDC(hdcSrc);
	}
	return hbmNew;// return our transformed bitmap.
}

HBITMAP CEditor::loadTransparentBitmap(HINSTANCE hInstance, int resource) {
	return MakeBitMapTransparent(LoadBitmap(hInstance, MAKEINTRESOURCE(resource)));
}

void CEditor::createToolbar() {

	// Declare and initialize local constants.
	const int bitmapSize = 16;

	HINSTANCE hInstance = ::GetModuleHandle(0);

	handleToolbar = CreateToolbarEx(handle, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
		0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, NULL, 0, 0, 0, 0, 0, sizeof(TBBUTTON));

	//Enable multiple image lists
	SendMessage(handleToolbar, CCM_SETVERSION, (WPARAM)5, 0);

	//Add icons from default imagelist
	TBBUTTON tbb_buildin[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb_buildin), (LPARAM)&tbb_buildin);

	//Create custom imagelist
	HIMAGELIST hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, 0, 0);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_RECTANGLE), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_ELLIPSE), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_TEXTBOX), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_PLAY), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_DELETE), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap( hInstance, IDB_EDIT ), NULL);
	SendMessage(handleToolbar, TB_SETIMAGELIST, (WPARAM)1, (LPARAM)hImageList);

	TBBUTTON tbb[] =
	{
		{ MAKELONG(0, 1), ID_ADD_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(1, 1), ID_ADD_ELLIPSE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(2, 1), ID_ADD_TEXT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(3, 1), ID_PLAY_LAUNCHPLAYER, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(4, 1), ID_DELETE_OBJECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(5, 1), ID_EDIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb), (LPARAM)&tbb);

	SendMessage(handleToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(handleToolbar, TRUE);
}

void CEditor::OnSize()
{
	RECT rect;
	int middleX, nWidth, nHeight;
	int RELATIVE_WIDTH = 3;
	int RELATIVE_HEIGHT = 4;
	int RELATIVE_TOOLBAR = 16;
	::GetClientRect( handle, &rect );
	middleX = (rect.left + rect.right) / RELATIVE_HEIGHT;
	nWidth = (rect.right - rect.left) * RELATIVE_WIDTH / RELATIVE_HEIGHT;

	RECT toolbarRect;
	::GetClientRect(handleToolbar, &toolbarRect);
	int currentTop = rect.top + (toolbarRect.bottom - toolbarRect.top);
	nHeight = (rect.bottom - currentTop);

	SetWindowPos( renderingWindow.GetHandle(), HWND_TOP, middleX, currentTop, nWidth, nHeight, 0 );
	int widthOfButton = nWidth / RELATIVE_WIDTH;
	int yOfButton = nHeight * RELATIVE_WIDTH / RELATIVE_HEIGHT;
	int cyOfButton = nHeight / RELATIVE_TOOLBAR;
	SetWindowPos( editControl.GetHandle(), HWND_TOP, rect.left, currentTop, widthOfButton, yOfButton, 0 );
	SetWindowPos( setFontButton, HWND_TOP, rect.left, currentTop + yOfButton, widthOfButton, cyOfButton, 0 );
	SetWindowPos( saveTextButton, HWND_TOP, rect.left, currentTop + yOfButton + cyOfButton, widthOfButton, cyOfButton, 0 );
	SetWindowPos( setColorButton, HWND_TOP, rect.left, currentTop + yOfButton + cyOfButton * 2, widthOfButton, cyOfButton, 0 );
	SetWindowPos( addScriptButton, HWND_TOP, rect.left, currentTop + yOfButton + cyOfButton * 3, widthOfButton, cyOfButton, 0 );

	SendMessage(handleToolbar, TB_AUTOSIZE, 0, 0);
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
            addObject( std::make_shared<CRectangleObject>( defaultObjectColor, generateDefaultBox() ) );
			break;
		}
		case ID_ADD_ELLIPSE:
		{
            addObject( std::make_shared<CEllipseObject>( defaultObjectColor, generateDefaultBox() ) );
			break;
		}
		case ID_ADD_TEXT:
		{
            addObject( std::make_shared<CTextBoxObject>( defaultObjectColor, generateDefaultBox(), L"Text" ) );
			break;
		}
		case ID_FILE_SAVE:
		{
			OnSave( );
			break;
		}
		case ID_PLAY_LAUNCHPLAYER:
		{
			CViewer viewer( *stage, CViewport() );
            if( viewer.Create() ) {
                const CViewerWindow& viewerWindow = viewer.GetViewerWindow();
                HWND viewerHandle = viewerWindow.GetHandle();
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
		case ID_EDIT:
		{

			CScriptEditor scriptEditor;

			if( scriptEditor.Create() ) {
				HWND scriptEditorWindow = scriptEditor.GetHandle();
				MSG message;
				BOOL getMessageResult = 0;
				while( (getMessageResult = GetMessage( &message, scriptEditorWindow, 0, 0 )) != 0 ) {
					if( getMessageResult == -1 ) {
						break;
					}
					TranslateMessage( &message );
					DispatchMessage( &message );
				}
			}
			break;
		}
		case ID_DELETE_OBJECT:
		{
			stage->GetObjects().erase(activeId);
			SetActiveId( CObjectIdGenerator::GetEmptyId() );
			renderingWindow.ReDraw();
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
			else if ((HWND)lParam == setFontButton) {
				onFontSelect();
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

template<class ObjectClass>
void CEditor::addObject( std::shared_ptr<ObjectClass> object )
{
    bool isAdded = stage->AddObject( object->GetId(), object );
    if( isAdded ) {
        SetActiveId( object->GetId() );
    }
    renderingWindow.ReDraw();
}

TBox CEditor::generateDefaultBox() const
{
	TBox box;
	RECT rect;
	GetWindowRect( renderingWindow.GetHandle(), &rect );
	box.right = 3 * ( box.left = ( rect.right - rect.left ) / defaultBoxMarginDividor);
	box.bottom = 3 * ( box.top = ( rect.bottom - rect.top ) / defaultBoxMarginDividor);
	return stage->GetViewPort().ConvertToModelCoordinates( box );
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

void CEditor::onFontSelect()
{
	CHOOSEFONT chooseFont;
	static LOGFONT logFont;
	HFONT hFont;
	static DWORD rgbCurrent;
	ZeroMemory(&chooseFont, sizeof(chooseFont));
	chooseFont.lStructSize = sizeof(chooseFont);
	chooseFont.hwndOwner = handle;
	chooseFont.lpLogFont = &logFont;
	chooseFont.rgbColors = rgbCurrent;
	chooseFont.Flags = CF_SCREENFONTS | CF_EFFECTS;
	if (::ChooseFont(&chooseFont)) {
		hFont = CreateFontIndirect(chooseFont.lpLogFont);
		//stage->GetObjectById(activeId)->SetColor(chooseFont.rgbColors);
		renderingWindow.ReDraw();
	}
}

void CEditor::OnSave( ) const
{
	OPENFILENAMEW filename;
	filename.lStructSize = sizeof( filename );
	filename.hwndOwner = handle;
	filename.hInstance = GetModuleHandleW( nullptr );
	filename.lpstrFilter = L"PS project (*.ps)\0*.ps\0All Files (*.*)\0*.*\0\0";
	filename.lpstrCustomFilter = 0;
	filename.nFilterIndex = 1;
	filename.nMaxFile = 1024;
	std::unique_ptr<wchar_t[]> name( new wchar_t[filename.nMaxFile] );
	filename.lpstrFile = name.get( );
	filename.lpstrFile[0] = 0;
	filename.lpstrInitialDir = 0;
	filename.lpstrTitle = L"Save project";
	filename.nMaxFileTitle = 0;
	filename.lpstrDefExt = L".ps";
	filename.lCustData = 0;
	filename.lpfnHook = 0;
	filename.lpTemplateName = 0;
	filename.nFileOffset = 0;
	filename.nFileExtension = 0;
	filename.Flags = OFN_OVERWRITEPROMPT;

	if ( ::GetSaveFileName( &filename ) ) {
		std::wofstream saveStream( filename.lpstrFile, std::ofstream::out );
		saveStream << stage->ToWString();
		saveStream.close( );
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

