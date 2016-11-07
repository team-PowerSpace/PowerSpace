#include "stdafx.h"
#include "ObjectIdGenerator.h"

std::unordered_map<std::string, int> CObjectIdGenerator::typeCounter;

std::string CObjectIdGenerator::generateNewIdByTypeName( const std::string& typeName )
{
    int newIntId = typeCounter[typeName]++;
    return typeName + std::to_string( newIntId );
}
