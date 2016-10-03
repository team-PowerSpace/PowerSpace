#pragma once

#include "Drawable.h"

class Rectangle : public IDrawable {
public:
	virtual void Draw( HDC paintDc ) override;
};