#include <stdafx.h>
#include "Stage.h"
#include "JsonConverter.h"

const std::unordered_map<int, IDrawablePtr>& CStage::GetObjects() const
{
	return objects;
}

std::unordered_map<int, IDrawablePtr>& CStage::GetObjects()
{
	return objects;
}

std::vector<IDrawablePtrConst> CStage::GetObjectsAsVector() const
{
	std::vector<IDrawablePtrConst> result;
	result.reserve( objects.size() );
	for( const std::pair< int, IDrawablePtr> pair : objects ) {
		result.push_back( pair.second );
	}
	return result;
}

IDrawablePtrConst CStage::GetObjectById( int objectId ) const
{
	return objects.at( objectId );
}

IDrawablePtr CStage::GetObjectById( int objectId )
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

void CStage::ClipAndDrawObjects( HDC hdc ) const
{
    std::vector<IDrawablePtrConst> objectList = GetObjectsAsVector();
    ClipAndDrawObjects(hdc, objectList);
}

CViewport & CStage::GetViewPort()
{
	return viewport;
}

const CViewport & CStage::GetViewPort() const
{
	return viewport;
}

std::wstring CStage::ToWString( ) const
{
	return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CStage::ToJson() const
{
	return CJsonConverter::ToJsonObject( *this );
}
