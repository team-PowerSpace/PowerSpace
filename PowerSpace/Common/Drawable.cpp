#include <stdafx.h>
#include "Drawable.h"

//Event All should be carefully changed with click, when the model of scripts will be defined
CDrawable::CDrawable( COLORREF _color, TBox _box, double _angle, bool needGenerateId = true )
	: color( _color ), containingBox( _box ), angle( _angle ),  scripts( )
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

void CDrawable::AddScript( CScript script )
{
	scripts.push_back( script );
}

const std::vector<CScript>& CDrawable::GetScripts( ) const
{
	return scripts;
}

CDrawable::DrawableType CDrawable::GetType() const
{
	return DrawableType::CDrawable;
}