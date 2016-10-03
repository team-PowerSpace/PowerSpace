#include "stdafx.h"
#include "ScriptEngine.h"


CScriptEngine::CScriptEngine( CStage& _stage )
    : stage( _stage )
{}

std::vector<int> CScriptEngine::RunScripts( int objectId, const std::vector<CScript>& scripts )
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( objectId );
    UNREFERENCED_PARAMETER( scripts );
    return std::vector<int>();
}
