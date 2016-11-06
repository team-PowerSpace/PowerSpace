#include <stdafx.h>
#include "StageObjects.h"

CRectangleObject::CRectangleObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box )
{}

void CRectangleObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawRectangle( hdc, viewport.ConvertToScreenCoordinates(containingBox), color );
}

std::wstring CRectangleObject::toWString( ) const
{
	return L"Rect description";
}


CEllipseObject::CEllipseObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box )
{}

void CEllipseObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawEllipse( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color );
}

std::wstring CEllipseObject::toWString( ) const
{
	return L"Ellipse description";
}


CTextBoxObject::CTextBoxObject( COLORREF _color, TBox _box, const std::wstring& _contents )
    : CDrawable( _color, _box ), contents( _contents )
{}

void CTextBoxObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawTextBox( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color, contents );
}

std::wstring CTextBoxObject::GetContents( ) const
{
	return contents;
}

void CTextBoxObject::SetContents( const std::wstring &_contents )
{
	contents = _contents;
}

std::wstring CTextBoxObject::toWString( ) const
{
	return L"Text description";
}
