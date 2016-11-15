#include "stdafx.h"
#include "ProtertyEditor.h"

ProtertyEditor::ProtertyEditor()
{
}

ProtertyEditor::~ProtertyEditor()
{
}

bool ProtertyEditor::Create( HWND _parentHandle, IDrawablePtr drawableObjectPtr )
{
	parentHandle = _parentHandle;
	CreateDialogParam( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDD_DIALOG1 ), parentHandle, dialogProc, reinterpret_cast<LPARAM>(this) );
	ShowWindow( handle, SW_SHOW );
	//MessageBox( 0, L"sdfsdf", 0, 0 );
	return (handle != 0);
}

HWND ProtertyEditor::getHandle()
{
	return handle;
}

void ProtertyEditor::OnInit( HWND _handle )
{
	handle = _handle;
	//SendDlgItemMessage( handle, IDC_SLIDER1, TBM_SETRANGEMIN, true, 8 );
	//SendDlgItemMessage( handle, IDC_SLIDER1, TBM_SETRANGEMAX, true, 72 );
	
	// Учимся получать данные объекта
	//ProtertyEditor* parent = reinterpret_cast<ProtertyEditor*>(GetWindowLongPtr( parentHandle, GWLP_USERDATA ));
	//LOGFONT logFont;
	//actualFont = reinterpret_cast<HFONT>(SendMessage( editControlHandle, WM_GETFONT, 0, 0 ));
	//GetObject( actualFont, sizeof( logFont ), &logFont );
	//oldParameters.font = newParameters.font = logFont;
	//oldParameters.opacity = newParameters.opacity = parent->getOpacity();
	//oldParameters.fontColor = newParameters.fontColor = parent->getFontColor();
	//oldParameters.backgroundColor = newParameters.backgroundColor = parent->getBackgroundColor();

	//SendDlgItemMessage( handle, IDC_SLIDER1, TBM_SETPOS, true, (LPARAM)logFont.lfHeight );
}

void ProtertyEditor::applyNewSettings( const PropertiesParameters& _settings )
{
	//ProtertyEditor* parent = reinterpret_cast<ProtertyEditor*>(GetWindowLongPtr( parentHandle, GWLP_USERDATA ));
	//DeleteObject( actualFont );
	//actualFont = CreateFontIndirect( &_settings.font );
	//SendMessage( parentHandle, WM_SETFONT, reinterpret_cast<WPARAM>(actualFont), true );
	//SetLayeredWindowAttributes( parentHandle, 0, _settings.opacity, LWA_ALPHA );
	//parent->setColors( _settings.backgroundColor, _settings.fontColor );
	//parent->setOpacity( _settings.opacity );	
}

void ProtertyEditor::selectColor( COLORREF& _color )
{
	CHOOSECOLOR chooseColor = {};
	chooseColor.lStructSize = sizeof( CHOOSECOLOR );
	chooseColor.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
	chooseColor.rgbResult = _color;
	chooseColor.hwndOwner = handle;
	chooseColor.lpCustColors = &_color;
	::ChooseColor( &chooseColor );
	_color = chooseColor.rgbResult;
}

INT_PTR ProtertyEditor::OnCommand( WPARAM wParam )
{
	//switch LOWORD( wParam )
	//{
	//	case IDC_BUTTON1:
	//	{
	//		selectColor( newParameters.backgroundColor );
	//		CheckDlgButton( handle, IDC_CHECK1, BST_UNCHECKED );
	//		break;
	//	}
	//	case IDC_BUTTON2:
	//	{
	//		selectColor( newParameters.fontColor );
	//		CheckDlgButton( handle, IDC_CHECK1, BST_UNCHECKED );
	//		break;
	//	}
	//	case IDC_CHECK1:
	//	{
	//		if( IsDlgButtonChecked( handle, IDC_CHECK1 ) == BST_CHECKED ) {
	//			applyNewSettings( newParameters );
	//		}
	//		break;
	//	}
	//	case IDOK:
	//	{
	//		applyNewSettings( newParameters );
	//		EndDialog( handle, wParam );
	//		return true;
	//	}
	//	case IDCANCEL:
	//	{
	//		applyNewSettings( oldParameters );
	//		EndDialog( handle, wParam );
	//		return true;
	//	}
	//}
	//
	return false;
}

//Слайдер размера шрифта. Пока без него.
//void ProtertyEditor::OnScroll( WPARAM wParam, LPARAM lParam )
//{
//	HWND commandHandle = reinterpret_cast<HWND>(lParam);
//	if( commandHandle == GetDlgItem( handle, IDC_SLIDER1 ) ) {
//		newParameters.font.lfHeight = SendDlgItemMessage( handle, IDC_SLIDER1, TBM_GETPOS, 0, 0 );
//	}
//	CheckDlgButton( handle, IDC_CHECK1, BST_UNCHECKED );
//}

INT_PTR ProtertyEditor::dialogProc( HWND _handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_INITDIALOG ) {
		ProtertyEditor* dialog = reinterpret_cast<ProtertyEditor*>(lParam);
		SetWindowLongPtr( _handle, GWLP_USERDATA, reinterpret_cast<LONG>(dialog) );
		dialog->OnInit( _handle );
		return true;
	}
	ProtertyEditor* dialog = reinterpret_cast<ProtertyEditor*>(GetWindowLongPtr( _handle, GWLP_USERDATA ));
	switch( message ) {
		case WM_COMMAND:
		{
			return dialog->OnCommand( wParam );
			break;
		}
		//case WM_HSCROLL:
		//{
		//	dialog->OnScroll( wParam, lParam );
		//	return false;
		//}
	}
	return false;
}
