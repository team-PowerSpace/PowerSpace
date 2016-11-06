#include <stdafx.h>
#include "Script.h"
#include "JsonConverter.h"

CScript::CScript( TPath _path )
    : path( _path )
{}

const TPath& CScript::GetPath() const
{
    return path;
}

std::wstring CScript::toWString( ) const
{
	return CJsonConverter::toJson( *this );
}
