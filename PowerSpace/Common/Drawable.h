#pragma once
#include <Script.h>
#include <memory>
#include <unordered_map>
#include <JsonObject.h>
#include <ObjectIdGenerator.h>

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
    virtual const IdType& GetId() const = 0;

    // getter and setter for containingBox field
    virtual TBox GetContainingBox() const = 0;
    virtual void SetContainingBox( TBox box ) = 0;

    // getter and setter for color field
    virtual COLORREF GetColor() const = 0;
    virtual void SetColor( COLORREF color ) = 0;

    // getter and setter for color field
    virtual double GetAngle() const = 0;
    virtual void SetAngle( double angle ) = 0;

    // draws object on canvas
    virtual void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const = 0;

    // links new script to the object
    virtual void AddScript( EventType eventType, CScript script ) = 0;
    virtual const std::vector<CScript>& GetScripts( EventType eventType ) const = 0;

    enum DrawableType
    {
        CDrawable, count
    };
    virtual DrawableType GetType() const = 0;

    virtual std::wstring ToWString() const = 0;
    virtual IJsonPtr ToJson() const = 0;
};

// this abstract class should be the base class for every object that can be drawn on the stage
class CDrawable : public IDrawable
{
public:
    CDrawable( COLORREF _color, TBox _box, double _angle, bool needGenerateId, COLORREF _borderColor = RGB(0, 0, 0) );
    CDrawable( COLORREF _color, TBox _box, double _angle, const std::unordered_map<EventType, std::vector<CScript>>& _scripts,
        bool needGenerateId, COLORREF _borderColor = RGB( 0, 0, 0 ) );

    virtual ~CDrawable() {}

    const IdType& GetId() const;

    TBox GetContainingBox() const;
    void SetContainingBox( TBox newPosition );

    COLORREF GetColor() const;
    void SetColor( COLORREF newColor );

    double GetAngle() const;
    void SetAngle( double angle );

    virtual void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const = 0;

    // links new script to the object
    void AddScript( EventType eventType, CScript script );

    // getters for scripts field
    const std::vector<CScript>& GetScripts( EventType eventType ) const;

    virtual DrawableType GetType() const;
protected:
    // a unique identifier linked to the object
    IdType id;

    // color of the object (default color of the object)
    COLORREF color;

    // color of the border of the object (if it has any)
    COLORREF borderColor;

    // the rectangle (position and size) that contains the object on stage
    TBox containingBox;

    // rotation of the object
    double angle;

    // maps event type to a list of scripts that are linked to the object;
    // this map supports the idea that an object can have multiple scripts attached to an event type
    std::unordered_map<EventType, std::vector<CScript>> scripts;
};
