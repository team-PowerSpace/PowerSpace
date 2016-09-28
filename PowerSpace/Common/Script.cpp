#include "../stdafx.h"
#include "Script.h"

CScript::CScript( TPath _path )
    : path( _path )
{}

TPath CScript::GetPath() const
{
    return path;
}
