#pragma once
#include <Stage.h>
#include <CScriptHolder.h>
#include <CDrawableBuilder.h>
#include <map>
// this class processes python scripts and changes the stage
class CScriptEngine
{
public:
    explicit CScriptEngine( CStage& _stage );

	void LoadScene();
	void AddPyObject( IdType name, IDrawablePtr description );
	void AddPyScripts();
    // evaluates python scripts from "scripts", modifying the stage;
    // objectId - id of the object, which caused the event
    // returns the vector of objects that were modified during the evaluation of the scripts
    void RunScripts(const IdType& objectId, EventType type, const std::vector<IdType>& scripts);
private:
	bool isPythonRunning; // boolean parameter to set when we start the python running
    CStage& stage;
	std::map<IdType, std::shared_ptr<CDrawableBuilder>> pyScene;
	PyObject* globalDictionary;	
};
