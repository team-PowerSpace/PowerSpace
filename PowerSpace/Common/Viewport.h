#pragma once
#include <Drawable.h>

// contains all the information about the viewport (zoom, drag), 
// makes different transformations connected with the viewport
class CViewport
{
public:
    // default ctorm which set zero location to (0,0) and scale to 1
    CViewport();
    CViewport( double _scale, double _angle, TPoint _zeroLocation );

    // checks if objects in "objects" are visible on the canvas with the current viewport;
    // returns vector of indices of (fully or partially) visible objects
    std::vector<int> ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const;

    // converts model coordinates (i.e. coordinates on stage) of an object 
    // to screen coordinates (i.e. coordinates on canvas)
    TBox ConvertToScreenCoordinates( const TBox& box ) const;

    // converts screen coordinates (i.e. coordinates on canvas) of an object 
    // to model coordinates (i.e. coordinates on stage)
    TBox ConvertToModelCoordinates( const TBox& box ) const;

    // converts model coordinates (i.e. coordinates on stage)
    // to screen coordinates (i.e. coordinates on canvas)
    TPoint ConvertToScreenCoordinates( const TPoint& point ) const;

    // converts screen coordinates (i.e. coordinates on canvas)
    // to model coordinates (i.e. coordinates on stage)
    TPoint ConvertToModelCoordinates( const TPoint& point ) const;

    const TPoint& GetZeroLocation() const;
    void SetZeroLocation( const TPoint& );

    double GetScale() const;
    double GetAngle() const;
    void SetScale( const double );
    void SetAngle( const double );

private:
    // coeff of scaling
    double scale;
    // angle of rotation
    double angle;
    // location of model point (0, 0) in screen coordinates
    TPoint zeroLocation;
};

