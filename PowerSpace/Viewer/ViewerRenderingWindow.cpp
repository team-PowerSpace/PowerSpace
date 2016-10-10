#include "stdafx.h"
#include "ViewerRenderingWindow.h"

CViewerRenderingWindow::CViewerRenderingWindow() :
	windowHeight( 600 ), windowWidth( 800 )
{
}

CViewerRenderingWindow::~CViewerRenderingWindow()
{
	// virtual destructor
}

void CViewerRenderingWindow::Show() const
{
	::ShowWindow( handle, SW_MAXIMIZE );
}

HWND CViewerRenderingWindow::GetHandle() const
{
	return handle;
}

LRESULT CViewerRenderingWindow::WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_NCCREATE ) {
		SetWindowLongPtr( handle, 0, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) );

		return DefWindowProc( handle, msg, wParam, lParam );
	}

	CViewerRenderingWindow* wndPtr = reinterpret_cast<CViewerRenderingWindow*>(GetWindowLongPtr( handle, 0 ));
	
	switch( msg ) {
		case WM_DESTROY:
			wndPtr->OnDestroy();
			break;

		default:
			break;
	}

	return DefWindowProc( handle, msg, wParam, lParam );
}

bool CViewerRenderingWindow::Create( const wchar_t * classname )
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	handle = CreateWindowEx( 0, classname, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowHeight, windowWidth, 0, 0, hInstance, this );

	return (handle != 0);
}

void CViewerRenderingWindow::OnDestroy() {
	::PostQuitMessage( 0 );
}