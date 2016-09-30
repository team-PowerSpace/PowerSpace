#pragma once
#include "Drawable.h"

// this class stores everything that was created on canvas;
// it is a logical representation of canvas
class CStage
{
public:
    // getters of objects field
    const std::unordered_map<int, IDrawablePtr>& CStage::GetObjects() const;
    std::unordered_map<int, IDrawablePtr>& GetObjects();

    // get object by its id
    IDrawablePtrConst GetObjectById( int objectId ) const;
    IDrawablePtr GetObjectById( int objectId );
private:
    // vector of all the objects that were created on canvas
    std::unordered_map<int, IDrawablePtr> objects;
};
