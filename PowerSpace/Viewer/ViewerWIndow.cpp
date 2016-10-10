#include "stdafx.h"
#include "ViewerWindow.h"

const wchar_t* CViewerWindow::ClassName = L"CViewerWindow";

CViewerWindow::CViewerWindow( CViewport& _viewport, CCanvas& _canvas ) :
	windowHeight( 600 ), windowWidth( 800 ), viewport(_viewport), canvas(_canvas), handle(nullptr)
{
}

CViewerWindow::~CViewerWindow()
{
}

bool CViewerWindow::RegisterClass()
{
	HBRUSH backgroundBrush = reinterpret_cast<HBRUSH> (::GetStockObject( WHITE_BRUSH ));
	HMODULE instance = GetModuleHandleW( nullptr );

	WNDCLASSEX windowClassInforamtion;
	windowClassInforamtion.cbSize = sizeof( WNDCLASSEX );
	windowClassInforamtion.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassInforamtion.lpfnWndProc = WindowProc;
	windowClassInforamtion.cbClsExtra = 0;
	windowClassInforamtion.cbWndExtra = 2 * sizeof( LONG_PTR );
	windowClassInforamtion.hInstance = instance;
	windowClassInforamtion.hIcon = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );
	windowClassInforamtion.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClassInforamtion.hbrBackground = backgroundBrush;
	windowClassInforamtion.lpszMenuName = 0;
	windowClassInforamtion.lpszClassName = ClassName;
	windowClassInforamtion.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );

	return (::RegisterClassEx( &windowClassInforamtion ) != 0);
}

bool CViewerWindow::Create()
{
	HINSTANCE hInstance = GetModuleHandle( nullptr );

	handle = CreateWindowEx( 0, ClassName, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowHeight, windowWidth, 0, 0, hInstance, this );

	return (handle != 0);
}

LRESULT CViewerWindow::WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );

		return DefWindowProc( handle, msg, wParam, lParam );
	}

	CViewerWindow* wndPtr = reinterpret_cast<CViewerWindow*>(GetWindowLongPtr( handle, 0 ));

	switch( msg ) {
	case WM_DESTROY:
		wndPtr->OnDestroy();
		break;

	default:
		break;
	}

	return DefWindowProc( handle, msg, wParam, lParam );
}

void CViewerWindow::OnDestroy()
{
	::PostQuitMessage( 0 );
}

void CViewerWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CViewerWindow::GetHandle() const
{
	return handle;
}
