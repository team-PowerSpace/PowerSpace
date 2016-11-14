#pragma once
#include <Stage.h>
#include <CScriptHolder.h>

// this class processes python scripts and changes the stage
class CScriptEngine
{
public:
    explicit CScriptEngine( CStage& _stage );

    // evaluates python scripts from "scripts", modifying the stage;
    // objectId - id of the object, which caused the event
    // returns the vector of objects that were modified during the evaluation of the scripts
    std::vector<int> RunScripts(IdType objectId, EventType type, std::vector<PyObject*> scripts);
private:
	bool isPythonRunning; // boolean parameter to set when we start the python running
    CStage& stage;
};
