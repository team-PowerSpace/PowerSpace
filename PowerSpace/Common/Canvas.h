#pragma once
#include <Drawable.h>

const COLORREF noColor = RGB( 200, 150, 150 );

class CCanvas
{
public:
	// draws rectangle on canvas
	// TODO: replace IDrawable with the class that implements IDrawable
	void DrawRectangle( HDC hdc, TBox box, COLORREF color ) const;

	// draws circle on canvas
	void DrawEllipse( HDC hdc, TBox box, COLORREF color ) const;

	// draws text on canvas
	void DrawTextBox( HDC hdc, TBox box, COLORREF color, const std::wstring& text, HFONT hFont,
		UINT format = DT_CENTER | DT_SINGLELINE | DT_VCENTER ) const;
};

