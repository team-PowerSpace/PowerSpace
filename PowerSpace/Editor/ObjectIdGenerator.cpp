#include <stdafx.h>
#include "ObjectIdGenerator.h"

std::unordered_map<std::wstring, int> CObjectIdGenerator::typeCounter;

IdType CObjectIdGenerator::GetEmptyId()
{
    return std::wstring();
}

IdType CObjectIdGenerator::generateNewIdByTypeName( const std::wstring& typeName )
{
    int newIntId = typeCounter[typeName]++;
    return typeName + std::to_wstring( newIntId );
}
