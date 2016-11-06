#include <stdafx.h>
#include "JsonConverter.h"
#include <sstream>

JSON CJsonConverter::toJson( const CStage &stage, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n" , depth);
	++depth;
	addPropertyToDescription( description, L"Objects", toJson( stage.GetObjectsAsVector( ) ), depth );
	addPropertyToDescription( description, L"ViewPort", toJson( stage.GetViewPort( ) ), depth );
	--depth;
	return description;
}

JSON CJsonConverter::toJson( const std::vector<IDrawablePtrConst> &objects, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"[", depth );
	int numberOfObjects = objects.size( );
	for ( int index = 0; index < numberOfObjects - 1; ++index ) {
		addElementToDescription( description, objects[index]->toWString( ) + L",\n", depth + 1 );
	}
	addElementToDescription( description, objects[numberOfObjects - 1]->toWString( ), depth + 1 );
	addElementToDescription( description, L"]", depth );
	return description;
}

JSON &CJsonConverter::addElementToDescription( JSON &description, const JSON &element, int depth )
{
	std::wstringstream sstream( element );
	std::wstring row;
	while ( std::getline( sstream, row ) ) {
		description += std::wstring( depth, '\t' ) + row;
	}
	return description;
}

JSON CJsonConverter::toJson( const CViewport &viewPort, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth);
	++depth;
	addPropertyToDescription( description, L"Scale", std::to_wstring( viewPort.GetScale( ) ), depth );
	addPropertyToDescription( description, L"Zero location", toJson( viewPort.GetZeroLocation( ) ), depth );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CRectangleObject &rectangle, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	++depth;
	addPropertyToDescription( description, L"Id", std::to_wstring( rectangle.GetId( ) ), depth );
	addPropertyToDescription( description, L"Color", std::to_wstring( rectangle.GetColor( ) ), depth );
	addPropertyToDescription( description, L"Box", toJson( rectangle.GetContainingBox( ) ), depth );
	addPropertyToDescription( description, L"Scripts", 
		toJson( rectangle.GetScripts( EventType::EventAll ) ), depth, LAST_PROP );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CTextBoxObject &text, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	++depth;
	addPropertyToDescription( description, L"Id", std::to_wstring( text.GetId( ) ), depth );
	addPropertyToDescription( description, L"Color", std::to_wstring( text.GetColor( ) ), depth );
	addPropertyToDescription( description, L"Box", toJson( text.GetContainingBox( ) ), depth );
	addPropertyToDescription( description, L"Scripts",
		toJson( text.GetScripts( EventType::EventAll ) ), depth );
	addPropertyToDescription( description, L"Content", text.GetContents( ), depth, LAST_PROP );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CEllipseObject &ellipse, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	++depth;
	addPropertyToDescription( description, L"Id", std::to_wstring( ellipse.GetId( ) ), depth );
	addPropertyToDescription( description, L"Color", std::to_wstring( ellipse.GetColor( ) ), depth );
	addPropertyToDescription( description, L"Box", toJson( ellipse.GetContainingBox( ) ), depth );
	addPropertyToDescription( description, L"Scripts",
		toJson( ellipse.GetScripts( EventType::EventAll ) ), depth, LAST_PROP );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const TBox &box, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	++depth;
	addPropertyToDescription( description, L"Top", std::to_wstring(box.top), depth );
	addPropertyToDescription( description, L"Bottom", std::to_wstring(box.bottom), depth );
	addPropertyToDescription( description, L"Left", std::to_wstring(box.left), depth );
	addPropertyToDescription( description, L"Right", std::to_wstring(box.right), depth, LAST_PROP );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const TPoint &point, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	++depth;
	addPropertyToDescription( description, L"X", std::to_wstring(point.x), depth );
	addPropertyToDescription( description, L"Y", std::to_wstring(point.y), depth );
	--depth;
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const std::vector<CScript> &objects, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"[", depth );
	int numberOfObjects = objects.size( );
	for ( int index = 0; index < numberOfObjects - 1; ++index ) {
		addElementToDescription( description, objects[index].toWString( ) + L",\n", depth + 1 );
	}
	addElementToDescription( description, objects[numberOfObjects - 1].toWString( ), depth + 1 );
	addElementToDescription( description, L"]", depth );
	return description;
}

JSON CJsonConverter::toJson( const CScript &script, int depth )
{
	JSON description = L"";
	addElementToDescription( description, L"{\n", depth );
	addPropertyToDescription( description, L"Path", script.GetPath( ), depth + 1, LAST_PROP );
	addElementToDescription( description, L"}", depth );
	return description;
}

JSON &CJsonConverter::addPropertyToDescription(
	JSON &description, const std::wstring &name, const JSON &prop, int depth, int last )
{
	addElementToDescription( description, L"\"" + name + L"\" : ", depth );
	addElementToDescription( description, L"\"" + prop + L"\"", depth );
	if ( last == 0 ) {
		addElementToDescription( description, L",\n", depth );
	}
	else {
		addElementToDescription( description, L"\n", depth );
	}
	return description;
}