#pragma once
#include "Drawable.h"

class CCanvas
{
public:
    // draws rectangle on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawRectangle( HDC hdc, TBox box, COLORREF color ) const;

    // draws circle on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawEllipse( HDC hdc, TBox box, COLORREF color ) const;

    // draws text on canvas
    // TODO: replace IDrawable with the class that implements IDrawable
    void DrawTextBox( HDC hdc, TBox box, COLORREF color ) const;
};

