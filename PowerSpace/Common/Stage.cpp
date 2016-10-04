#include <stdafx.h>
#include "Stage.h"

const std::unordered_map<int, IDrawablePtr>& CStage::GetObjects() const
{
    return objects;
}

std::unordered_map<int, IDrawablePtr>& CStage::GetObjects()
{
    return objects;
}

IDrawablePtrConst CStage::GetObjectById( int objectId ) const
{
    return objects.at( objectId );
}

IDrawablePtr CStage::GetObjectById( int objectId )
{
    return objects.at( objectId );
}

void CStage::DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const
{
    for( const IDrawablePtrConst& object : objectList ) {
        object->Draw( hdc, viewport, canvas );
    }
}

void CStage::ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const
{
    std::vector<int> objectsClippedIndices = viewport.ClipObjects( objectList );
    std::vector<IDrawablePtrConst> objectsClipped;
    objectsClipped.reserve( objectsClippedIndices.size() );
    for( int i : objectsClippedIndices ) {
        objectsClipped.push_back( objectList[i] );
    }
    DrawObjects( hdc, objectsClipped );
}
