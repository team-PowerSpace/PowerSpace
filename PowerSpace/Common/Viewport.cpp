#include <stdafx.h>
#include "Viewport.h"

std::vector<int> CViewport::ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( objects );
    return std::vector<int>();
}

TBox CViewport::ConvertToCoordinates( TBox box ) const
{
    // TODO: implement this method
    return box;
}
