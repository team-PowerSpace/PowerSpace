#pragma once

typedef std::wstring JSON;
typedef std::wstring JsonKey;
typedef std::wstring JsonContent;
typedef std::wstring JsonName;

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
	CJsonArray( const JsonName& _name = L"" );

	JSON ToJson( int depth = 0 ) const;

	std::vector<IJsonPtr> objects;

};

class CJsonWString : public IJsonObject
{
public:
	virtual ~CJsonWString();
	CJsonWString( const JsonContent& _content = L"", const JsonName& _name = L"" );
	JSON ToJson( int depth = 0 ) const;

	JsonContent content;
};

class CJsonMap : public IJsonObject
{
public:
	virtual ~CJsonMap();
	CJsonMap( const JsonName& _name = L"" );
	JSON ToJson( int depth = 0 ) const;

	std::unordered_map<JsonKey, IJsonPtr> objects;
};
