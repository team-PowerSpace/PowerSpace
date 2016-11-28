#include <stdafx.h>
#include "Drawable.h"

//Event All should be carefully changed with click, when the model of scripts will be defined
CDrawable::CDrawable( COLORREF _color, TBox _box, double _angle, bool needGenerateId = true, COLORREF _borderColor )
	: color( _color ), containingBox( _box ), angle( _angle ),  scripts( { {EventType::EventAll/*Should be EventClick*/, std::vector<CScript>()},
	{EventType::EventAll/*Should be EventTick*/, std::vector<CScript>()} } ), borderColor( _borderColor )
{
	if( needGenerateId ) {
		id = CObjectIdGenerator::GenerateNewId<CDrawable>();
	}
}

CDrawable::CDrawable( COLORREF _color, TBox _box, double _angle, const std::unordered_map<EventType, std::vector<CScript>>& _scripts, bool needGenerateId = true, COLORREF _borderColor)
    : color( _color ), containingBox( _box ), angle( _angle ), scripts( _scripts ), borderColor( _borderColor )
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

double CDrawable::GetAngle() const
{
	return angle;
}

void CDrawable::SetAngle( double newAngle )
{
	angle = newAngle;
}

void CDrawable::AddScript( EventType eventType, CScript script )
{
	scripts[eventType].push_back( script );
}

const std::vector<CScript>& CDrawable::GetScripts( EventType eventType ) const
{
	return scripts.at( eventType );
}

CDrawable::DrawableType CDrawable::GetType() const
{
	return DrawableType::CDrawable;
}