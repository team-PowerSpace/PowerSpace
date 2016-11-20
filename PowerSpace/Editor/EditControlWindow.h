#pragma once
#include <Windows.h>

class CEditControlWindow
{
public:
	CEditControlWindow();
	~CEditControlWindow();

	// create edit control window
	bool Create( HWND parentHandle );

	// getter of handle field
	HWND GetHandle();

private:
	void initializeText();
	HWND handle;
};