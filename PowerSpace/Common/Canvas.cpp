#include <stdafx.h>
#include "Canvas.h"

void CCanvas::DrawRectangle( HDC hdc, TBox box, COLORREF color ) const
{
	HBRUSH brush = ::CreateSolidBrush( color );
	if( color != noColor) {
		::FillRect( hdc, &box, brush );
	}
	::DeleteObject( brush );
}

void CCanvas::DrawEllipse( HDC hdc, TBox box, COLORREF color ) const
{
	HBRUSH brush = ::CreateSolidBrush( color );
	HBRUSH brushOld = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
	if( color != noColor ) {
		::Ellipse( hdc, box.left, box.top, box.right, box.bottom );
	}
	::SelectObject( hdc, brushOld );
	::DeleteObject( brush );
}

void CCanvas::DrawTextBox( HDC hdc, TBox box, COLORREF color, const std::wstring& text, HFONT hFont, UINT format ) const
{
	int bkModeOld = ::SetBkMode( hdc, TRANSPARENT );
	COLORREF colorOld = ::SetTextColor( hdc, color );
	::SelectObject( hdc, hFont );
	::DrawText( hdc, text.c_str(), -1, &box, format );
	::SetTextColor( hdc, colorOld );
	::SetBkMode( hdc, bkModeOld );
}

