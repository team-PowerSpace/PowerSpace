#include <stdafx.h>
#include "StageObjects.h"
#include <JsonConverter.h>

CRectangleObject::CRectangleObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box, false )
{
    id = CObjectIdGenerator::GenerateNewId<CRectangleObject>();
}

void CRectangleObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawRectangle( hdc, viewport.ConvertToScreenCoordinates(containingBox), color );
}

std::wstring CRectangleObject::ToWString( ) const
{
	return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CRectangleObject::ToJson() const
{
	return CJsonConverter::ToJsonObject( *this );
}

CEllipseObject::CEllipseObject( COLORREF _color, TBox _box )
    : CDrawable( _color, _box, false )
{
    id = CObjectIdGenerator::GenerateNewId<CEllipseObject>();
}

void CEllipseObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawEllipse( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color );
}

std::wstring CEllipseObject::ToWString( ) const
{
	return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CEllipseObject::ToJson() const
{
	return CJsonConverter::ToJsonObject( *this );
}


CTextBoxObject::CTextBoxObject( COLORREF _color, TBox _box, const std::wstring& _contents )
    : CDrawable( _color, _box, false ), contents( _contents )
{
    id = CObjectIdGenerator::GenerateNewId<CTextBoxObject>();
}

void CTextBoxObject::Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const
{
    canvas.DrawTextBox( hdc, viewport.ConvertToScreenCoordinates( containingBox ), color, contents, font );
}

std::wstring CTextBoxObject::GetContents( ) const
{
	return contents;
}

void CTextBoxObject::SetContents( const std::wstring &_contents )
{
	contents = _contents;
}

void CTextBoxObject::SetFont( HFONT newFont )
{
	font = newFont;
}

std::wstring CTextBoxObject::ToWString( ) const
{
	return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CTextBoxObject::ToJson() const
{
	return CJsonConverter::ToJsonObject( *this );
}
