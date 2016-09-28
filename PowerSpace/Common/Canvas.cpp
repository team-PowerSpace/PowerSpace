#include "../stdafx.h"
#include "Canvas.h"

void CCanvas::DrawObjects( HDC hdc, const std::vector<IDrawable>& objects ) const
{
    for( const IDrawable& object : objects ) {
        object.Draw( hdc );
    }
}

void CCanvas::ClipAndDrawObjects( HDC hdc, const std::vector<IDrawable>& objects ) const
{
    std::vector<int> objectsClippedIndices = ClipObjects( objects );
    std::vector<IDrawable> objectsClipped;
    objectsClipped.reserve( objectsClippedIndices.size() );
    for( int i : objectsClippedIndices ) {
        objectsClipped.push_back( objects[i] );
    }
    DrawObjects( hdc, objectsClipped );
}
