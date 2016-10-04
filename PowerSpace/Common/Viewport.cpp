#include "../stdafx.h"
#include "Viewport.h"


std::vector<int> CViewport::ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( objects );
    return std::vector<int>();
}

RECT CViewport::ConvertToCoordinates( TPosition logicalPosition ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( logicalPosition );
    RECT rect = {};
    return rect;
}
