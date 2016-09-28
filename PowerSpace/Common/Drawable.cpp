#include "../stdafx.h"
#include "Drawable.h"

int CDrawable::maxId = 0;

CDrawable::CDrawable( COLORREF _color, TPosition _position )
    : color( _color ), position( _position )
{
    id = generateNewId();
}

int CDrawable::GetId() const
{
    return id;
}

TPosition CDrawable::GetPosition() const
{
    return position;
}
void CDrawable::SetPosition( TPosition newPosition )
{
    position = newPosition;
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
    std::vector<CScript> result;
    auto scriptsIt = scripts.find( eventType );
    if( scriptsIt != scripts.end() ) {
        result = scriptsIt->second;
    }
    return result;
}

std::vector<CScript> CDrawable::GetScripts( EventType eventType )
{
    return scripts[eventType];
}

int CDrawable::generateNewId()
{
    return maxId++;
}
