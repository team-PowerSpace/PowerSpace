#include <stdafx.h>
#include "Script.h"
#include <JsonConverter.h>
#include <fstream>

std::unordered_map<EventType, IdType> CScript::Events = {
    { EventType::EventClick, L"onClick" },
    { EventType::EventTick, L"onTick" }
};

CScript::CScript( TPath _path )
{
    std::ifstream stream( _path.data(), std::ifstream::in ); //The best way to check path validity is trying to open it
    if( !stream.good() ) {
        stream.close();
        std::cout << "The file doesn't exist" << std::endl;
        assert( false );
    }
    stream.close();

    //Deleteing the path and saving only script's filename (with extention)
    std::wstring scriptName = _path.substr( _path.find_last_of( L"\\/" ) + 1 );

    //Deleting the extention of script
    name = scriptName.substr( 0, scriptName.find( L"." ) );
}

const TPath& CScript::GetPath() const
{
	return path;
}

ScriptName CScript::GetName() const
{
    return name;
}

std::wstring CScript::ToWString() const
{
	return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CScript::ToJson() const
{
	return CJsonConverter::ToJsonObject( *this );
}
