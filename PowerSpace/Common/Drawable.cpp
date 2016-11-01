#include <stdafx.h>
#include "Drawable.h"

int CDrawable::maxId = 0;

//Event All should be carefully changed with click, when the model of scripts will be defined
CDrawable::CDrawable( COLORREF _color, TBox _box )
    : color( _color ), containingBox( _box ), scripts( { {EventType::EventAll/*Should be EventClick*/, std::vector<CScript>()}, 
	{EventType::EventAll/*Should be EventTick*/, std::vector<CScript>( )} } )
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

const std::vector<CScript>& CDrawable::GetScripts( EventType eventType ) const
{
    return scripts.at(eventType);
}

int CDrawable::generateNewId()
{
    return ++maxId;
}

CDrawable::DrawableType CDrawable::GetType() const {
	return DrawableType::CDrawable;
}