#include <stdafx.h>
#include "EditControlWindow.h"

CEditControlWindow::CEditControlWindow()
{
	handle = 0;
}

CEditControlWindow::~CEditControlWindow()
{}

void CEditControlWindow::initializeText()
{
	HMODULE module = GetModuleHandle( 0 );
	UNREFERENCED_PARAMETER( module );
	// TODO: implement this method
	SetWindowText( handle, reinterpret_cast<LPCWSTR>("") );
}

bool CEditControlWindow::Create( HWND parentHandle )
{
	handle = CreateWindowEx( 0, L"EDIT", 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle( 0 ), 0 );
	initializeText();
	SetFocus( handle );
	return handle != 0;
}

HWND CEditControlWindow::GetHandle()
{
	return handle;
}
