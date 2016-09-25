#pragma once
#include <Windows.h>

class IDrawable {
public:
    virtual ~IDrawable() {}
protected:
    int id;
    COLORREF color;
    RECT position;
    //transform;
};
