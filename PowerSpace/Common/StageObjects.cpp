#include <stdafx.h>
#include "StageObjects.h"

CRectangleObject::CRectangleObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box )
{}

void CRectangleObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawRectangle( hdc, viewport.ConvertToScreenCoordinates(containingBox), color );
}

CEllipseObject::CEllipseObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box )
{}

void CEllipseObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawEllipse( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color );
}

CTextBoxObject::CTextBoxObject( COLORREF _color, TBox _box, const std::wstring& _contents )
    : CDrawable( _color, _box ), contents( _contents )
{}

void CTextBoxObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawTextBox( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color, contents );
}
