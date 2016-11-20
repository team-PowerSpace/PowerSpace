#include <stdafx.h>
#include "Canvas.h"

void CCanvas::Rotate( HDC hdc, double angle ) const
{
	//double radians = (2 * 3.1416 * angle) / 360;
	double radians = angle;
	double cosinus = (double)cos( radians );
	double sinus = (double)sin( radians );
	XFORM xForm;
	xForm.eM11 = (FLOAT)cosinus;
	xForm.eM12 = (FLOAT)sinus;
	xForm.eM21 = (FLOAT)-sinus;
	xForm.eM22 = (FLOAT)cosinus;
	xForm.eDx = (FLOAT) 0.0;
	xForm.eDy = (FLOAT) 0.0;
	SetWorldTransform( hdc, &xForm );
}

TBox CCanvas::MakeBoxCentreCoordinatesOrigin( HDC hdc, TBox box ) const
{
	SetViewportOrgEx( hdc, (box.left + box.right) / 2, (box.top + box.bottom) / 2, NULL );

	TBox box_transformed;
	box_transformed.left = box.left - (box.left + box.right) / 2;
	box_transformed.right = box.right - (box.left + box.right) / 2;
	box_transformed.top = box.top - (box.top + box.bottom) / 2;
	box_transformed.bottom = box.bottom - (box.top + box.bottom) / 2;

	return box_transformed;
}

void CCanvas::DrawRectangle( HDC hdc, TBox box, COLORREF color, double angle ) const
{
	Rotate( hdc, angle );
	TBox box_transformed = MakeBoxCentreCoordinatesOrigin( hdc, box );

    HBRUSH brush = ::CreateSolidBrush( color );
	if( color != noColor) {
		::FillRect( hdc, &box_transformed, brush );
	}
    ::DeleteObject( brush );

	Rotate( hdc, 0 );
	SetViewportOrgEx( hdc, 0, 0, NULL );
}

void CCanvas::DrawEllipse( HDC hdc, TBox box, COLORREF color, double angle ) const
{
	Rotate( hdc, angle );
	TBox box_transformed = MakeBoxCentreCoordinatesOrigin( hdc, box );

    HBRUSH brush = ::CreateSolidBrush( color );
    HBRUSH brushOld = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
	if( color != noColor ) {
		::Ellipse( hdc, box_transformed.left, box_transformed.top, box_transformed.right, box_transformed.bottom );
	}
    ::SelectObject( hdc, brushOld );
    ::DeleteObject( brush );

	Rotate( hdc, 0 );
	SetViewportOrgEx( hdc, 0, 0, NULL );
}

void CCanvas::DrawTextBox( HDC hdc, TBox box, COLORREF color, double angle, const std::wstring& text, HFONT hFont, UINT format ) const
{
	Rotate( hdc, angle );
	TBox box_transformed = MakeBoxCentreCoordinatesOrigin( hdc, box );

	int bkModeOld = ::SetBkMode( hdc, TRANSPARENT );
    COLORREF colorOld = ::SetTextColor( hdc, color );
	::SelectObject( hdc, hFont );
	::DrawText( hdc, text.c_str(), -1, &box_transformed, format );
    ::SetTextColor( hdc, colorOld );
	::SetBkMode( hdc, bkModeOld );

	Rotate( hdc, 0 );
	SetViewportOrgEx( hdc, 0, 0, NULL );
}

