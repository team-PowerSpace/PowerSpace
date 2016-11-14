#pragma once

using JSON = std::wstring;
using JsonKey = std::wstring;
using JsonContent = std::wstring;
using JsonName = std::wstring;
using JsonObjectBody = std::wstring;
using JsonPosition = size_t;
using JsonTag = wchar_t;
enum JsonObjectType { SINGLE, LIST, MAP, COUNT };
using JsonObjectDescription = struct
{
    JsonName name;
    JsonPosition nameBeginPosition;
    JsonPosition nameEndPosition;
    JsonObjectBody body;
    JsonPosition bodyBeginPosition;
    JsonPosition bodyEndPosition;
    JsonObjectType type;
};

class IJsonObject
{
public:
    virtual ~IJsonObject() {}

    virtual JSON ToJson( int depth = 0 ) const = 0;

    JsonName name;
};

using IJsonPtr = std::shared_ptr<IJsonObject>;


class CJsonArray : public IJsonObject
{
public:
    virtual ~CJsonArray();
    CJsonArray( const JsonName& _name, const JsonObjectBody& _body );
    CJsonArray( const JsonName& _name = L"" );

    JSON ToJson( int depth = 0 ) const;

    std::vector<IJsonPtr> objects;

};

class CJsonWString : public IJsonObject
{
public:
    virtual ~CJsonWString();
    CJsonWString( const JsonName& _name, const JsonObjectBody& _body );
    JSON ToJson( int depth = 0 ) const;

    JsonContent content;
};

class CJsonMap : public IJsonObject
{
public:
    virtual ~CJsonMap();
    CJsonMap( const JsonName& _name = L"" );
    CJsonMap( const JsonName& _name, const JsonObjectBody& _body );
    JSON ToJson( int depth = 0 ) const;

    std::unordered_map<JsonKey, IJsonPtr> objects;
};
