#pragma once
#include <Python.h>
#include <unordered_map>
#include <codecvt>
#include <Exception>
#include <iostream>
#include <fstream>
#include <CScriptBuilder.h>
#include <ObjectIdGenerator.h>


class CScriptHolder
{
    //key -> name of object
    //value -> all scripts of type associated with the object
    std::unordered_map<IdType, std::vector<IdType>, IdTypeHash> Scripts; //Scripts, that were run on this session

public:
    std::vector<IdType> getScripts( const IdType &key ) const;				 //Get scripts by objectId
    bool addScript( const IdType& key, const TPath& script );		//Add script by it's path
    void removeScripts( const IdType& objectId );	//Remove scripts when object removed

    size_t getSize() const;
};