#include <stdafx.h>
#include <JsonObject.h>
#include <JsonWorker.h>
#include <JsonTreeVisitor.h>

CJsonArray::CJsonArray( const JsonName& _name )
{
    name = _name;
}

CJsonArray::CJsonArray( const JsonName& _name, const JsonObjectBody& _body )
{
    name = _name;
    objects = CJsonWorker::ReadObjects( _body );
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

void CJsonArray::Accept( IJsonTreeVisitor& visitor ) const
{
    visitor.Visit( this );
}

CJsonWString::CJsonWString( const JsonName& _name, const JsonObjectBody& _body )
{
    name = _name;
    content = _body;
}

JSON CJsonWString::ToJson( int depth ) const
{
    JSON description;
    CJsonWorker::AddRowToDescription( description, L"\"" + name + L"\" : ", depth );
    CJsonWorker::AddRowToDescription( description, L"\"" + content + L"\"\n", 0 );
    return description;
}

void CJsonWString::Accept( IJsonTreeVisitor& visitor ) const
{
    visitor.Visit( this );
}

CJsonMap::CJsonMap( const JsonName& _name )
{
    name = _name;
}

CJsonMap::CJsonMap( const JsonName& _name, const JsonObjectBody& _body )
{
    name = _name;
    auto objectPtrs = CJsonWorker::ReadObjects( _body );
    for( auto objectPtr : objectPtrs ) {
        objects.insert( std::make_pair( objectPtr->name, objectPtr ) );
    }
}

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

void CJsonMap::Accept( IJsonTreeVisitor& visitor ) const
{
    visitor.Visit( this );
}
