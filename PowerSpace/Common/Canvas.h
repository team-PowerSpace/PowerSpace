#pragma once
#include "Drawable.h"

class CCanvas
{
public:
    // draws rectangle on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawRectangle( HDC hdc, IDrawablePtrConst rectangle ) const;

    // draws circle on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawCircle( HDC hdc, IDrawablePtrConst circle ) const;

    // draws text on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawText( HDC hdc, IDrawablePtrConst text ) const;

    // draws all objects in "objects" on canvas
    void DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objects ) const;

    // checks if the objects in "objects" are visible on the canvas with the current viewport;
    // returns vector of indices of (fully or partially) visible objects
    std::vector<int> ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const;

    // draws only visible objects from "objects" on canvas
    void ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objects ) const;

    // converts logical coordinates (i.e. coordinates on stage) of an object 
    // to real coordinates (i.e. coordinates on canvas)
    RECT ConvertToCoordinates( TPosition logicalPosition ) const;
};

