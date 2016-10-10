#include <stdafx.h>
#include "Drawable.h"

int CDrawable::maxId = 0;

CDrawable::CDrawable( COLORREF _color, TBox _box )
    : color( _color ), containingBox( _box )
{
    id = generateNewId();
}

int CDrawable::GetId() const
{
    return id;
}

TBox CDrawable::GetContainingBox() const
{
    return containingBox;
}

void CDrawable::SetContainingBox( TBox newBox )
{
    containingBox = newBox;
}

COLORREF CDrawable::GetColor() const
{
    return color;
}

void CDrawable::SetColor( COLORREF newColor )
{
    color = newColor;
}

void CDrawable::AddScript( EventType eventType, CScript script )
{
    scripts[eventType].push_back( script );
}

const std::vector<CScript>& CDrawable::GetScripts( EventType eventType )
{
    return scripts[eventType];
}

int CDrawable::generateNewId()
{
    return maxId++;
}

CDrawable::DrawableType CDrawable::GetType() {
	return DrawableType::CDrawable;
}