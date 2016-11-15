#include <stdafx.h>
#include <JsonWorker.h>
#include <sstream>
#include <string>
//Because of an error in VS which cause by initializing of typeFromTag
#pragma warning( disable : 4592 )

const JsonName CJsonWorker::EMPTY_NAME = L"";
const JsonObjectBody CJsonWorker::EMPTY_BODY = L"";


const std::unordered_map<JsonTag, JsonObjectType> CJsonWorker::typeFromTag = {
    { L'{', JsonObjectType::MAP },
    { L'}', JsonObjectType::MAP },
    { L'[', JsonObjectType::LIST },
    { L']', JsonObjectType::LIST },
    { L'\"', JsonObjectType::SINGLE }
};

JSON &CJsonWorker::AddElementToDescription( JSON& description, const JSON& element, int depth, std::wstring sep )
{
    std::wstringstream sstream( element );
    std::wstring row;
    while( std::getline( sstream, row ) ) {
        description += std::wstring( depth, '\t' ) + row + L"\n";
    }
    description.pop_back();
    description += sep;
    return description;
}

JSON& CJsonWorker::AddRowToDescription( JSON& description, const JSON& row, int depth )
{
    return description += std::wstring( depth, '\t' ) + row;
}

int CJsonWorker::OpenTag( JSON& description, const std::wstring& tag, int depth )
{
    description += L"\n" + std::wstring( depth++, L'\t' ) + tag + L"\n";
    return depth;
}

int CJsonWorker::CloseTag( JSON& description, const std::wstring& tag, int depth )
{
    description += L"\n" + std::wstring( --depth, L'\t' ) + tag;
    return depth;
}

std::vector<std::shared_ptr<IJsonObject>> CJsonWorker::ReadObjects( const JSON& description )
{
    std::vector<std::shared_ptr<IJsonObject>> objects;
    int position = 0;
    JsonObjectDescription objectDescription = CJsonWorker::GetNextObjectDescription( description, position );
    while( objectDescription.name != EMPTY_NAME ) {
        switch( objectDescription.type ) {
        case JsonObjectType::LIST:
        {
            objects.push_back( std::make_shared<CJsonArray>( objectDescription.name, objectDescription.body ) );
            break;
        }
        case JsonObjectType::SINGLE:
        {
            objects.push_back( std::make_shared<CJsonWString>( objectDescription.name, objectDescription.body ) );
            break;
        }
        case JsonObjectType::MAP:
        {
            objects.push_back( std::make_shared<CJsonMap>( objectDescription.name, objectDescription.body ) );
            break;
        }
        default:
            assert( JsonObjectType::COUNT == 3 );
        }
        objectDescription = GetNextObjectDescription( description, objectDescription.bodyEndPosition + 2 );
    }
    return objects;
}

JsonObjectDescription CJsonWorker::GetNextObjectDescription( const JSON& description, JsonPosition position )
{
    JsonObjectDescription objectDescription = {};

    objectDescription.nameBeginPosition = description.find_first_of( L"\"", position );
    if( objectDescription.nameBeginPosition == std::wstring::npos ) {
        objectDescription.name = EMPTY_NAME;
        objectDescription.body = EMPTY_BODY;
        return objectDescription;
    } else {
        ++objectDescription.nameBeginPosition;
    }
    objectDescription.nameEndPosition = description.find_first_of( L"\"", objectDescription.nameBeginPosition );

    int nameSize = static_cast<int>(objectDescription.nameEndPosition - objectDescription.nameBeginPosition);
    objectDescription.name = description.substr( objectDescription.nameBeginPosition, nameSize );
    objectDescription.bodyBeginPosition = description.find_first_of( L"\"[{", objectDescription.nameEndPosition + 1 ) + 1;

    position = objectDescription.bodyBeginPosition;
    objectDescription.type = CJsonWorker::typeFromTag.at( description[position - 1] );
    std::stack<JsonTag> operators;
    operators.push( description[position - 1] );
    ++position;
    while( !operators.empty() && position < description.size() ) {
        position = description.find_first_of( L"\"[]{}", position );
        CJsonWorker::updateOperatorsStack( operators, description[position] );
        ++position;
    }
    int bodySize = static_cast<int>(position - 1 - objectDescription.bodyBeginPosition);
    if( operators.empty() ) {
        objectDescription.body = description.substr( objectDescription.bodyBeginPosition, bodySize );
    }
    objectDescription.bodyEndPosition = position - 2;

    return objectDescription;
}

void CJsonWorker::updateOperatorsStack( std::stack<JsonTag>& operators, JsonTag newTag )
{
    JsonTag topTag = operators.top();
    if( typeFromTag.at( topTag ) != typeFromTag.at( newTag ) ) {
        operators.push( newTag );
    } else if( topTag == newTag ) {
        if( topTag == L'\"' ) {
            operators.pop();
        } else {
            operators.push( newTag );
        }
    } else {
        operators.pop();
    }
}