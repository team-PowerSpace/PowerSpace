#pragma once
#include <Stage.h>
#include <CScriptHolder.h>
#include <map>
// this class processes python scripts and changes the stage
class CScriptEngine
{
public:
    explicit CScriptEngine( CStage& _stage );

	void LoadScene();
	void AddPyObject( IdType name, IDrawablePtr description );
    // evaluates python scripts from "scripts", modifying the stage;
    // objectId - id of the object, which caused the event
    // returns the vector of objects that were modified during the evaluation of the scripts
    std::vector<int> RunScripts(IdType objectId, EventType type, std::vector<std::shared_ptr<CScriptBuilder>> scripts);
private:
	bool isPythonRunning; // boolean parameter to set when we start the python running
    CStage& stage;
	std::map<IdType, PyObject*> pyScene;
	PyObject* globalDictionary;	
};
