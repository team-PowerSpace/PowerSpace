#include <stdafx.h>
#include "Canvas.h"

//void __fastcall
//vRotate( int viAngle )
//{
//	Graphics::TBitmap *pFromBitmap = new Graphics::TBitmap;
//	Graphics::TBitmap *pToBitmap = new Graphics::TBitmap;
//	pFromBitmap->LoadFromFile( "a1.bmp" );
//	//√еометрические преобразовани€ коорденат дл€
//	//определени€ положени€ пикселей источника
//	float vfRradians = (2 * 3.1416*viAngle) / 360;
//	float vfCosinus = (float)cos( vfRradians );
//	float vfSinus = (float)sin( vfRradians );
//	float vfPoint1x = (-pFromBitmap->Height*vfSinus);
//	float vfPoint1y = (pFromBitmap->Height*vfCosinus);
//	float vfPoint2x = (pFromBitmap->Width*vfCosinus - pFromBitmap->Height*vfSinus);
//	float vfPoint2y = (pFromBitmap->Height*vfCosinus + pFromBitmap->Width*vfSinus);
//	float vfPoint3x = (pFromBitmap->Width*vfCosinus);
//	float vfPoint3y = (pFromBitmap->Width*vfSinus);
//	float vfMinx = min( 0, min( vfPoint1x, min( vfPoint2x, vfPoint3x ) ) );
//	float vfMiny = min( 0, min( vfPoint1y, min( vfPoint2y, vfPoint3y ) ) );
//	float vfMaxX = max( vfPoint1x, max( vfPoint2x, vfPoint3x ) );
//	float vfMaxY = max( vfPoint1y, max( vfPoint2y, vfPoint3y ) );
//	int pToBitmapWidth = (int)ceil( vfMaxX - vfMinx );
//	int pToBitmapHeight = (int)ceil( vfMaxY - vfMiny );
//	pToBitmap->Height = pToBitmapHeight;
//	pToBitmap->Width = pToBitmapWidth;
//	//ѕоворот с использованием функции SetWorldTransform
//	SetGraphicsMode( pToBitmap->Canvas->Handle, GM_ADVANCED );
//	XFORM xform;
//	xform.eM11 = vfCosinus;
//	xform.eM12 = vfSinus;
//	xform.eM21 = -vfSinus;
//	xform.eM22 = vfCosinus;
//	xform.eDx = (float)-vfMinx;
//	xform.eDy = (float)-vfMiny;
//	SetWorldTransform( pToBitmap->Canvas->Handle, &xform );
//	BitBlt( pToBitmap->Canvas->Handle, 0, 0,
//		//«десь коордената не приемника, а источника
//		//иначе при углах более 45 градусов изображение
//		//отобразитс€ урезанным
//		pFromBitmap->Width,
//		pFromBitmap->Height,
//		pFromBitmap->Canvas->Handle,
//		0,
//		0,
//		SRCCOPY );
//	//ќтображение повернутого образа
//	Image1->Left = 0;
//	Image1->Top = 0;
//	Image1->Width = pToBitmap->Width;
//	Image1->Height = pToBitmap->Height;
//	Image1->Picture->Bitmap = pToBitmap;
//	//»зображение можно сохранить
//	pToBitmap->SaveToFile( "a2.bmp" );
//}

void CCanvas::DrawRectangle( HDC hdc, TBox box, COLORREF color ) const
{
	XFORM xForm;
	xForm.eM11 = (FLOAT) 0.8660;
	xForm.eM12 = (FLOAT) 0.5000;
	xForm.eM21 = (FLOAT)-0.5000;
	xForm.eM22 = (FLOAT) 0.8660;
	xForm.eDx = (FLOAT) 0.0;
	xForm.eDy = (FLOAT) 0.0;
	SetWorldTransform( hdc, &xForm );
	HBRUSH brush = ::CreateSolidBrush( color );
	if( color != noColor) {
		::FillRect( hdc, &box, brush );
	}

	SetGraphicsMode( hdc, GM_ADVANCED );

	xForm.eM11 = (FLOAT) 1.0;
	xForm.eM12 = (FLOAT) 0.0;
	xForm.eM21 = (FLOAT) 0.0;
	xForm.eM22 = (FLOAT) 1.0;
	xForm.eDx = (FLOAT) 0.0;
	xForm.eDy = (FLOAT) 0.0;
	SetWorldTransform( hdc, &xForm );
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

