#include <stdafx.h>
#include "JsonObject.h"
#include <JsonWorker.h>

CJsonArray::~CJsonArray() {}

CJsonArray::CJsonArray( const JsonName& _name )
{
	name = _name;
}

JSON CJsonArray::ToJson( int depth ) const
{
	JSON description = L"\"" + name + L"\" : ";
	if( objects.size() == 0 ) {
		return description += L"\"None\"\n";
	}
	depth = CJsonWorker::OpenTag( description, L"[", depth );
	for( auto object : objects ) {
		CJsonWorker::AddElementToDescription( description, object->ToJson(), depth, L",\n" );
	}
	description.pop_back();
	description.pop_back();
	depth = CJsonWorker::CloseTag( description, L"]", depth );
	return description;
}

CJsonWString::CJsonWString( const JsonContent& _content, const JsonName& _name ) : content( _content )
{
	name = _name;
}

CJsonWString::~CJsonWString() {}

JSON CJsonWString::ToJson( int depth ) const
{
	JSON description;
	CJsonWorker::AddRowToDescription( description, L"\"" + name + L"\" : ", depth );
	CJsonWorker::AddRowToDescription( description, L"\"" + content + L"\"\n", 0 );
	return description;
}


CJsonMap::CJsonMap( const JsonName& _name )
{
	name = _name;
}

CJsonMap::~CJsonMap() {}


JSON CJsonMap::ToJson( int depth ) const
{
	JSON description = L"\"" + name + L"\" : ";
	if( objects.size() == 0 ) {
		return description += L"\"None\"\n";
	}
	depth = CJsonWorker::OpenTag( description, L"{", depth );
	for( auto object : objects ) {
		CJsonWorker::AddElementToDescription( description, object.second->ToJson(), depth, L",\n" );
	}
	description.pop_back();
	description.pop_back();
	depth = CJsonWorker::CloseTag( description, L"}", depth );
	return description;
}