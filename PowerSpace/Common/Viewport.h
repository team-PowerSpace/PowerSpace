#pragma once
#include "Drawable.h"

// contains all the information about the viewport (zoom, drag), 
// makes different transformations connected with the viewport
class CViewport
{
public:
    // checks if objects in "objects" are visible on the canvas with the current viewport;
    // returns vector of indices of (fully or partially) visible objects
    std::vector<int> ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const;

    // converts logical coordinates (i.e. coordinates on stage) of an object 
    // to real coordinates (i.e. coordinates on canvas)
    TBox ConvertToCoordinates( TBox box ) const;
};

