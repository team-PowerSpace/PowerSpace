#include <stdafx.h>
#include "JsonConverter.h"
#include <sstream>

JSON CJsonConverter::toJson( const CStage &stage, int depth )
{
	JSON description = L"";
	depth = openTag( description, L"{", depth );
	addCompoundPropertyToDescription( description, L"Objects", toJson( stage.GetObjectsAsVector( ) ), depth );
	addCompoundPropertyToDescription( description, L"ViewPort", toJson( stage.GetViewPort( ) ), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const std::vector<IDrawablePtrConst> &objects, int depth )
{
	JSON description = L"";
	int numberOfObjects = objects.size( );
	if ( numberOfObjects == 0 ) {
		return description;
	}
	depth = openTag( description, L"[", depth );
	for ( int index = 0; index < numberOfObjects - 1; ++index ) {
		addElementToDescription( description, objects[index]->toWString( ) + L",", depth, L"\n" );
	}
	addElementToDescription( description, objects[numberOfObjects - 1]->toWString( ), depth );
	depth = closeTag( description, L"]", depth );
	return description;
}

JSON CJsonConverter::toJson( const CViewport &viewPort, int depth )
{
	JSON description = L"";
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"Scale", std::to_wstring( viewPort.GetScale( ) ), depth );
	addCompoundPropertyToDescription( description, L"Zero location", toJson( viewPort.GetZeroLocation( ) ), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CRectangleObject &rectangle, int depth )
{
	JSON description = L"";
	addRowToDescription( description, L"\"Rectangle\" : ", depth );
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"Id", std::to_wstring( rectangle.GetId( ) ), depth);
	addSimplePropertyToDescription( description, L"Color", std::to_wstring( rectangle.GetColor( ) ), depth );
	addCompoundPropertyToDescription( description, L"Box", toJson( rectangle.GetContainingBox( ) ), depth );
	addCompoundPropertyToDescription( description, L"Scripts",
		toJson( rectangle.GetScripts( EventType::EventAll ) ), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CTextBoxObject &text, int depth )
{
	JSON description = L"";
	addRowToDescription( description, L"\"TextBox\" : ", depth );
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"Id", std::to_wstring( text.GetId( ) ), depth );
	addSimplePropertyToDescription( description, L"Color", std::to_wstring( text.GetColor( ) ), depth );
	addCompoundPropertyToDescription( description, L"Box", toJson( text.GetContainingBox( ) ), depth );
	addCompoundPropertyToDescription( description, L"Scripts",
		toJson( text.GetScripts( EventType::EventAll ) ), depth );
	addSimplePropertyToDescription( description, L"Content", text.GetContents( ), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const CEllipseObject &ellipse, int depth )
{
	JSON description = L"";
	addRowToDescription( description, L"\"Ellipse\" : ", depth );
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"Id", std::to_wstring( ellipse.GetId( ) ), depth );
	addSimplePropertyToDescription( description, L"Color", std::to_wstring( ellipse.GetColor( ) ), depth );
	addCompoundPropertyToDescription( description, L"Box", toJson( ellipse.GetContainingBox( ) ), depth );
	addCompoundPropertyToDescription( description, L"Scripts",
		toJson( ellipse.GetScripts( EventType::EventAll ) ), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const TBox &box, int depth )
{
	JSON description = L"";
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"Top", std::to_wstring(box.top), depth );
	addSimplePropertyToDescription( description, L"Bottom", std::to_wstring(box.bottom), depth );
	addSimplePropertyToDescription( description, L"Left", std::to_wstring(box.left), depth );
	addSimplePropertyToDescription( description, L"Right", std::to_wstring(box.right), depth, LAST_PROP );
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const TPoint &point, int depth )
{
	JSON description = L"";
	depth = openTag( description, L"{", depth );
	addSimplePropertyToDescription( description, L"X", std::to_wstring(point.x), depth );
	addSimplePropertyToDescription( description, L"Y", std::to_wstring(point.y), depth, LAST_PROP);
	depth = closeTag( description, L"}", depth );
	return description;
}

JSON CJsonConverter::toJson( const std::vector<CScript> &objects, int depth )
{
	JSON description = L"";
	int numberOfObjects = objects.size( );
	if ( numberOfObjects == 0 )
	{
		return description;
	}
	depth = openTag( description, L"[", depth );
	for ( int index = 0; index < numberOfObjects - 1; ++index ) {
		addElementToDescription( description, objects[index].toWString( ) + L",", depth, L"\n" );
	}
	addElementToDescription( description, objects[numberOfObjects - 1].toWString( ), depth );
	depth = closeTag( description, L"]", depth );
	return description;
}

JSON CJsonConverter::toJson( const CScript &script, int depth )
{
	JSON description = L"";
	addSimplePropertyToDescription( description, L"Path", script.GetPath( ), depth, LAST_PROP );
	return description;
}

JSON &CJsonConverter::addElementToDescription( JSON &description, const JSON &element, int depth, std::wstring sep )
{
	std::wstringstream sstream( element );
	std::wstring row;
	while ( std::getline( sstream, row ) ) {
		description += std::wstring( depth, '\t' )  + row + L"\n";
	}
	description.pop_back( );
	description += sep;
	return description;
}

JSON &CJsonConverter::addRowToDescription( JSON &description, const JSON &row, int depth )
{
	return description += std::wstring( depth, '\t' ) + row;
}


JSON &CJsonConverter::addSimplePropertyToDescription(
	JSON &description, const std::wstring &name, const JSON &prop, int depth, bool last )
{
	addRowToDescription( description, L"\"" + name + L"\" : ", depth );
	addRowToDescription( description, L"\"" + prop + L"\"", 0 );
	if ( !last ) {
		addRowToDescription( description, L",\n", 0 );
	}
	return description;
}

JSON &CJsonConverter::addCompoundPropertyToDescription(
	JSON &description, const std::wstring &name, const JSON &prop, int depth, bool last )
{
	addRowToDescription( description, L"\"" + name + L"\" : ", depth );
	addElementToDescription( description, prop, depth, L"" );
	if ( !last ) {
		addRowToDescription( description, L",\n", 0 );
	}
	return description;
}

template<> std::shared_ptr<CRectangleObject> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CRectangleObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CEllipseObject> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CEllipseObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CTextBoxObject> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CTextBoxObject>( RGB( 100, 90, 80 ), temp, L"Text" );
}

template<> std::shared_ptr<CViewport> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	return std::make_shared<CViewport>( );
}

int CJsonConverter::openTag( JSON &description, const std::wstring &tag, int depth )
{
	description += L"\n" + std::wstring(depth++, L'\t') + tag + L"\n";
	return depth;
}

int CJsonConverter::closeTag( JSON &description, const std::wstring &tag, int depth )
{
	description += L"\n" + std::wstring(--depth, L'\t') + tag;
	return depth;
}