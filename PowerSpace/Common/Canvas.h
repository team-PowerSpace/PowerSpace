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
};

