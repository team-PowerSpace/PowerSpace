#pragma once
#include "resource.h"
#include "Drawable.h"

class PropertiesParameters
{
public:
	LOGFONT font;
	COLORREF color;
};

class ProtertyEditor
{
public:
	ProtertyEditor();
	~ProtertyEditor();

	bool Create( HWND _parentHandle, IDrawablePtr drawableObjectPtr );
	HWND getHandle();

protected:

	void OnInit( HWND _handle );

	// Пока без скролла
	// void OnScroll( WPARAM wParam, LPARAM lParam );

	INT_PTR OnCommand( WPARAM wParam );

private:

	HWND handle;
	HWND parentHandle;

	PropertiesParameters oldParameters;
	PropertiesParameters newParameters;
	HFONT actualFont;

	void applyNewSettings( const PropertiesParameters& _settings );
	void selectColor( COLORREF& _color );

	static INT_PTR CALLBACK dialogProc( HWND hanndleDialog, UINT message, WPARAM wParam, LPARAM lParam );
};
