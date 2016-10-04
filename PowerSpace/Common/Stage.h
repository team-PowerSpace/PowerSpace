#pragma once
#include "Drawable.h"
#include "Viewport.h"
#include "Canvas.h"

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

    // draws all objects in "objects" on canvas
    void DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const;
    // draws only visible objects from "objects" on canvas
    void ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const;
private:
    // vector of all the objects that were created on canvas
    std::unordered_map<int, IDrawablePtr> objects;
    CViewport viewport;
    CCanvas canvas;
};