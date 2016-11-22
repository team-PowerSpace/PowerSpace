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

class IJsonTreeVisitor;

class IJsonObject
{
public:
    virtual ~IJsonObject() {}

    virtual JSON ToJson( int depth = 0 ) const = 0;
    virtual void Accept( IJsonTreeVisitor& visitor ) const = 0;
};

class CJsonObject : IJsonObject
{
public:
    virtual ~CJsonObject() {}

    virtual JSON ToJson( int depth = 0 ) const = 0;
    virtual void Accept( IJsonTreeVisitor& visitor ) const = 0;

    JsonName name;
};

using IJsonPtr = std::shared_ptr<CJsonObject>;

class CJsonArray : public CJsonObject
{
public:
    virtual ~CJsonArray() {}
    CJsonArray( const JsonName& _name, const JsonObjectBody& _body );
    CJsonArray( const JsonName& _name = L"" );

    JSON ToJson( int depth = 0 ) const;

    void Accept( IJsonTreeVisitor& visitor ) const override;

    std::vector<IJsonPtr> objects;

};

class CJsonWString : public CJsonObject
{
public:
    virtual ~CJsonWString() {}
    CJsonWString( const JsonName& _name, const JsonObjectBody& _body );
    JSON ToJson( int depth = 0 ) const;

    void Accept( IJsonTreeVisitor& visitor ) const override;

    JsonContent content;
};

class CJsonMap : public CJsonObject
{
public:
    virtual ~CJsonMap() {}
    CJsonMap( const JsonName& _name = L"" );
    CJsonMap( const JsonName& _name, const JsonObjectBody& _body );
    JSON ToJson( int depth = 0 ) const;

    void Accept( IJsonTreeVisitor& visitor ) const override;

    std::unordered_map<JsonKey, IJsonPtr> objects;
};
