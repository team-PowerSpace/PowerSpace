#include <stdafx.h>
#include "Drawable.h"

//Event All should be carefully changed with click, when the model of scripts will be defined
CDrawable::CDrawable( COLORREF _color, TBox _box, bool needGenerateId = true )
    : color( _color ), containingBox( _box ), scripts( { {EventType::EventAll/*Should be EventClick*/, std::vector<CScript>()},
	{EventType::EventAll/*Should be EventTick*/, std::vector<CScript>( )} } )
{
    if( needGenerateId ) {
        id = CObjectIdGenerator::GenerateNewId<CDrawable>();
    }
}

const IdType& CDrawable::GetId() const
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

CDrawable::DrawableType CDrawable::GetType() const {
	return DrawableType::CDrawable;
}