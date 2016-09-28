#pragma once
#include "Script.h"

// this type represents logical position of an object on the stage
using TPosition = RECT;

// this interface describes functional of every object that can be drawn on canvas
class IDrawable
{
public:
    virtual ~IDrawable() {}

    // returns a unique identifier linked to the object
    virtual int GetId() const = 0;

    // getter and setter for position field
    virtual TPosition GetPosition() const = 0;
    virtual void SetPosition( TPosition poisition ) = 0;

    // getter and setter for color field
    virtual COLORREF GetColor() const = 0;
    virtual void SetColor( COLORREF color ) = 0;

    // draws object on canvas
    virtual void Draw( HDC hdc ) const = 0;
};

// this abstract class should be the base class for every object that can be drawn on the stage
class CDrawable : public IDrawable
{
public:
    CDrawable( COLORREF _color, TPosition _position );
    virtual ~CDrawable() {}

    int GetId() const;

    TPosition GetPosition() const;
    void SetPosition( TPosition newPosition );

    COLORREF GetColor() const;
    void SetColor( COLORREF newColor );

    virtual void Draw( HDC hdc ) const = 0;

    // links new script to the object
    void AddScript( EventType eventType, CScript script );

    // getters for scripts field
    const std::vector<CScript>& GetScripts( EventType eventType ) const;
    std::vector<CScript> GetScripts( EventType eventType );
protected:
    // a unique identifier linked to the object
    int id;

    // color of the object
    COLORREF color;

    // logical position of the object on stage
    TPosition position;

    // maps event type to a list of scripts that are linked to the object;
    // this map supports the idea that an object can have multiple scripts attached to an event type
    std::unordered_map<EventType, std::vector<CScript>> scripts;
private:
    // generates a new unique identifier for the object
    int generateNewId();

    // counter of objects that have already been created; is used to generate new ids
    static int maxId;
};
