#include <stdafx.h>
#include "Script.h"
#include <JsonConverter.h>

CScript::CScript( TPath _path )
    : path( _path )
{}

const TPath& CScript::GetPath() const
{
    return path;
}

std::wstring CScript::ToWString() const
{
    return CJsonConverter::ToJsonObject( *this )->ToJson();
}

IJsonPtr CScript::ToJson() const
{
    return CJsonConverter::ToJsonObject( *this );
}
