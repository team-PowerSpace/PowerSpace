#include <stdafx.h>
#include "Viewport.h"

CViewport::CViewport() : scale( 1 ), angle( 0 ), zeroLocation()
{
    zeroLocation.x = 0;
    zeroLocation.y = 0;
}

std::vector<int> CViewport::ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const
{
    // TODO: implement this method; now it returns all the objects
    std::vector<int> visibleObjectsIndices;
    visibleObjectsIndices.reserve( objects.size() );
    for( unsigned int i = 0; i < objects.size(); ++i ) {
        visibleObjectsIndices.push_back( i );
    }
    return visibleObjectsIndices;
}

TBox CViewport::ConvertToScreenCoordinates( const TBox& box ) const
{
    TBox result;

    TPoint topleft;
    topleft.x = box.left;
    topleft.y = box.top;
    topleft = ConvertToScreenCoordinates( topleft );
    result.left = topleft.x;
    result.top = topleft.y;

    TPoint rightbottom;
    rightbottom.x = box.right;
    rightbottom.y = box.bottom;
    rightbottom = ConvertToScreenCoordinates( rightbottom );
    result.right = rightbottom.x;
    result.bottom = rightbottom.y;

    return result;
}

TBox CViewport::ConvertToModelCoordinates( const TBox& box ) const
{
    TBox result;

    TPoint topleft;
    topleft.x = box.left;
    topleft.y = box.top;
    topleft = ConvertToModelCoordinates( topleft );
    result.left = topleft.x;
    result.top = topleft.y;

    TPoint rightbottom;
    rightbottom.x = box.right;
    rightbottom.y = box.bottom;
    rightbottom = ConvertToModelCoordinates( rightbottom );
    result.right = rightbottom.x;
    result.bottom = rightbottom.y;

    return result;
}

TPoint CViewport::ConvertToScreenCoordinates( const TPoint & point ) const
{
    TPoint result;
    result.x = static_cast<long>(point.x * scale) + zeroLocation.x;
    result.y = static_cast<long>(point.y * scale) + zeroLocation.y;
    return result;
}

TPoint CViewport::ConvertToModelCoordinates( const TPoint & point ) const
{
    TPoint result;
    result.x = static_cast<long>((point.x - zeroLocation.x) / scale);
    result.y = static_cast<long>((point.y - zeroLocation.y) / scale);
    return result;
}

const TPoint & CViewport::GetZeroLocation() const
{
    return zeroLocation;
}

void CViewport::SetZeroLocation( const TPoint & value )
{
    zeroLocation = value;
}

float CViewport::GetScale() const
{
    return scale;
}

void CViewport::SetScale( const float value )
{
    scale = value;
}

float CViewport::GetAngle() const
{
    return angle;
}

void CViewport::SetAngle( const float value )
{
    angle = value;
}
