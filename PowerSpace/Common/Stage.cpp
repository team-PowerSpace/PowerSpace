#include <stdafx.h>
#include "Stage.h"

CStage::CStage( const std::unordered_map<IdType, IDrawablePtr>& _objects, const CViewport& _viewport )
    : objects(_objects), viewport( _viewport )
{}

const std::unordered_map<IdType, IDrawablePtr>& CStage::GetObjects() const
{
    return objects;
}

std::unordered_map<IdType, IDrawablePtr>& CStage::GetObjects()
{
    return objects;
}

bool CStage::AddObject( IdType objectId, IDrawablePtr object )
{
    return objects.insert( std::make_pair( objectId, object ) ).second;
}

std::vector<IDrawablePtrConst> CStage::GetObjectsAsVector() const
{
    std::vector<IDrawablePtrConst> result;
    result.reserve( objects.size() );
    for( const std::pair< IdType, IDrawablePtr> pair : objects ) {
        result.push_back( pair.second );
    }
    return result;
}

IDrawablePtrConst CStage::GetObjectById( IdType objectId ) const
{
    return objects.at( objectId );
}

IDrawablePtr CStage::GetObjectById( IdType objectId )
{
    return objects.at( objectId );
}

void CStage::DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const
{
    for( const IDrawablePtrConst& object : objectList ) {
        object->Draw( hdc, viewport, canvas );
    }
}

void CStage::DrawObjects( HDC hdc ) const
{
    for( auto pair : objects ) {
        pair.second->Draw( hdc, viewport, canvas );
    }
}

void CStage::DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList, const CViewport& thisViewport ) const
{
	for( const IDrawablePtrConst& object : objectList ) {
		object->Draw( hdc, thisViewport, canvas );
	}
}

void CStage::ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList ) const
{
    std::vector<int> objectsClippedIndices = viewport.ClipObjects( objectList );
    std::vector<IDrawablePtrConst> objectsClipped;
    objectsClipped.reserve( objectsClippedIndices.size() );
    for( int i : objectsClippedIndices ) {
        objectsClipped.push_back( objectList[i] );
    }
    DrawObjects( hdc, objectsClipped );
}

void CStage::ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objectList, const CViewport& thisViewport ) const
{
	std::vector<int> objectsClippedIndices = thisViewport.ClipObjects( objectList );
	std::vector<IDrawablePtrConst> objectsClipped;
	objectsClipped.reserve( objectsClippedIndices.size() );
	for( int i : objectsClippedIndices ) {
		objectsClipped.push_back( objectList[i] );
	}
	DrawObjects( hdc, objectsClipped, thisViewport );
}

void CStage::ClipAndDrawObjects( HDC hdc ) const
{
    std::vector<IDrawablePtrConst> objectList = GetObjectsAsVector();
    ClipAndDrawObjects( hdc, objectList );
}

void CStage::ClipAndDrawObjects( HDC hdc, const CViewport & thisViewport ) const
{
	std::vector<IDrawablePtrConst> objectList = GetObjectsAsVector();
	ClipAndDrawObjects( hdc, objectList, thisViewport );
}

CViewport & CStage::GetViewPort()
{
    return viewport;
}

const CViewport & CStage::GetViewPort() const
{
    return viewport;
}

std::wstring CStage::ToWString() const
{
    return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CStage::ToJson() const
{
    return CJsonConverter::ToJsonObject( *this );
}

void CStage::addScript( EventType type, IdType objectId, CScript script )
{
    scripts.addScript( std::pair<IdType, EventType>( objectId, type ), script );
}

std::vector<PyObject*> CStage::getScripts( IdType objId, EventType eventType )
{
    return scripts.getScript( std::pair<IdType, EventType>( objId, eventType ) );
}

void CStage::decScriptRefs()
{
    scripts.decAllRefs();
}

