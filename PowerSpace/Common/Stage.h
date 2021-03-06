#pragma once
#include <Canvas.h>
#include <Drawable.h>
#include <JsonConverter.h>
#include <Viewport.h>
#include <ObjectIdGenerator.h>
#include "CScriptHolder.h"

// this class stores everything that was created on canvas;
// it is a logical representation of canvas
class CStage
{
public:
    CStage() = default;
    CStage( const std::unordered_map<IdType, IDrawablePtr>& objects, const CViewport& viewport );

    // getters of objects field
    const std::unordered_map<IdType, IDrawablePtr>& CStage::GetObjects() const;
    std::unordered_map<IdType, IDrawablePtr>& GetObjects();

    bool AddObject( IdType objectId, IDrawablePtr object );

    // method for transforming objects to vector
    std::vector<IDrawablePtrConst> GetObjectsAsVector() const;

    // get object by its id
    IDrawablePtrConst GetObjectById( IdType objectId ) const;
    IDrawablePtr GetObjectById( IdType objectId );

	// draws all objects in "objectList" on canvas
	void DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const;
	// draws all objects in "objects" on canvas
	void DrawObjects( HDC hdc ) const;
	// draw all objects in specified viewport
	void DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList, const CViewport& thisViewport ) const;

	// draws only visible objects from "objectList" on canvas
	void ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const;
	// draws only visible objects from "objectList" on canvas (in viewport)
	void ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList, const CViewport& thisViewport ) const;
	// draws only visible objects from "objects" on canvas
	void ClipAndDrawObjects( HDC hdc ) const;
	// draw all objects in specified viewport
	void ClipAndDrawObjects( HDC hdc, const CViewport& thisViewport ) const;

    // not-const getter of viewport
    CViewport& GetViewPort();

    // getter of viewport
    const CViewport& GetViewPort() const;

    //add new script
    void addScript( EventType type, IdType objectId, CScript script );

    //get scripts by type of function and object
    std::vector<PyObject*> getScripts( IdType objId, EventType eventType );

    //script correct removal
    void decScriptRefs();

    // get the JSON representation of stage
    std::wstring ToWString() const;

    // Get the JSON object
    IJsonPtr ToJson() const;

private:
    // vector of all the objects that were created on canvas
    std::unordered_map<IdType, IDrawablePtr> objects;
    CViewport viewport;
    CCanvas canvas;
    ScriptHolder scripts;
};
