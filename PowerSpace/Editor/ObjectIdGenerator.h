#pragma once
#include <string>
#include <type_traits>
#include <unordered_map>

class CRectangleObject;
class CEllipseObject;
class CTextBoxObject;

using IdType = std::wstring;


struct IdTypeHash
{
	std::size_t operator()(IdType& k)
	{
		return (std::hash<std::wstring>()(k));
	}
};



// generates a new unique identifier for the object
class CObjectIdGenerator
{
public:
    CObjectIdGenerator() = default;
    ~CObjectIdGenerator() = default;

    static IdType GetEmptyId();

    // generic implementation
    template <typename T> static IdType GenerateNewId();

    // specific implementations
    template<> static IdType GenerateNewId<CRectangleObject>();
    template<> static IdType GenerateNewId<CEllipseObject>();
    template<> static IdType GenerateNewId<CTextBoxObject>();
private:
    static IdType generateNewIdByTypeName( const std::wstring& typeName );

    // counter of objects that have already been created; is used to generate new ids
    static std::unordered_map<std::wstring, int> typeCounter;
};

template<typename T> IdType CObjectIdGenerator::GenerateNewId()
{
    return generateNewIdByTypeName( L"object" );
}

template<> IdType CObjectIdGenerator::GenerateNewId<CRectangleObject>()
{
    return generateNewIdByTypeName( L"rect" );
}

template<> IdType CObjectIdGenerator::GenerateNewId<CEllipseObject>()
{
    return generateNewIdByTypeName( L"ellipse" );
}

template<> IdType CObjectIdGenerator::GenerateNewId<CTextBoxObject>()
{
    return generateNewIdByTypeName( L"text" );
}
