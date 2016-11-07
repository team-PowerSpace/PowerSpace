#pragma once
#include <string>
#include <type_traits>
#include <unordered_map>
#include <StageObjects.h>

class CObjectIdGenerator
{
public:
    CObjectIdGenerator() = default;
    ~CObjectIdGenerator() = default;

    // generic implementation
    template <typename T> static std::string GenerateNewId();

    // specific implementations
    template<> static std::string GenerateNewId<CRectangleObject>();
    template<> static std::string GenerateNewId<CEllipseObject>();
    template<> static std::string GenerateNewId<CTextBoxObject>();
private:
    static std::string generateNewIdByTypeName( const std::string& typeName );

    static std::unordered_map<std::string, int> typeCounter;
};

template<typename T> std::string CObjectIdGenerator::GenerateNewId()
{
    return generateNewIdByTypeName( "object" );
}

template<> std::string CObjectIdGenerator::GenerateNewId<CRectangleObject>()
{
    return generateNewIdByTypeName( "rect" );
}

template<> std::string CObjectIdGenerator::GenerateNewId<CEllipseObject>()
{
    return generateNewIdByTypeName( "ellipse" );
}

template<> std::string CObjectIdGenerator::GenerateNewId<CTextBoxObject>()
{
    return generateNewIdByTypeName( "text" );
}

