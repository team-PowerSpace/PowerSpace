#pragma once
#include "Script.h"
#include <memory>
#include <unordered_map>

class CCanvas;
class CViewport;
class IDrawable;

// this type represents logical position of an object on the stage
using TBox = RECT;
using TPoint = POINT;
using IDrawablePtr = std::shared_ptr<IDrawable>;
using IDrawablePtrConst = std::shared_ptr<const IDrawable>;

// this interface describes functional of every object that can be drawn on canvas
class IDrawable
{
public:
    virtual ~IDrawable() {}

    // returns a unique identifier linked to the object
    virtual int GetId() const = 0;

    // getter and setter for containingBox field
    virtual TBox GetContainingBox() const = 0;
    virtual void SetContainingBox( TBox box ) = 0;

    // getter and setter for color field
    virtual COLORREF GetColor() const = 0;
    virtual void SetColor( COLORREF color ) = 0;

    // draws object on canvas
    virtual void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const = 0;

    // links new script to the object
    virtual void AddScript( EventType eventType, CScript script ) = 0;

    enum DrawableType {
        CDrawable, count
    };
    virtual DrawableType GetType() const = 0;
};

// this abstract class should be the base class for every object that can be drawn on the stage
class CDrawable : public IDrawable
{
public:
    CDrawable( COLORREF _color, TBox _box );
    virtual ~CDrawable() {}

    int GetId() const;

    TBox GetContainingBox() const;
    void SetContainingBox( TBox newPosition );

    COLORREF GetColor() const;
    void SetColor( COLORREF newColor );

    virtual void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const = 0;

    // links new script to the object
    void AddScript( EventType eventType, CScript script );

    // getters for scripts field
    const std::vector<CScript>& GetScripts( EventType eventType );

	virtual DrawableType GetType() const;
protected:
    // a unique identifier linked to the object
    int id;

    // color of the object
    COLORREF color;

    // the rectangle (position and size) that contains the object on stage
    TBox containingBox;

    // maps event type to a list of scripts that are linked to the object;
    // this map supports the idea that an object can have multiple scripts attached to an event type
    std::unordered_map<EventType, std::vector<CScript>> scripts;
private:
    // generates a new unique identifier for the object
    int generateNewId();

    // counter of objects that have already been created; is used to generate new ids
    static int maxId;
};
