#include <stdafx.h>
#include "Viewport.h"

std::vector<int> CViewport::ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const
{
    // TODO: implement this method; now it returns all the objects
    std::vector<int> visibleObjectsIndices;
    visibleObjectsIndices.reserve(objects.size());
    for( unsigned int i = 0; i < objects.size(); ++i ) {
        visibleObjectsIndices.push_back(i);
    }
    return visibleObjectsIndices;
}

TBox CViewport::ConvertToCoordinates( TBox box ) const
{
    // TODO: implement this method, now it returns box
    return box;
}
