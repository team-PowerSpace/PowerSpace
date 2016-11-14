#include <stdafx.h>

#include "ScriptEngine.h"
#include <ScriptSolver.h>
#include <PyObjectBuilder.h>
#include <CDrawableBuilder.h>



CScriptEngine::CScriptEngine( CStage& _stage )
	: stage( _stage ), isPythonRunning( false )
{}



std::vector<int> CScriptEngine::RunScripts(IdType objectId, EventType type,  std::vector<PyObject*> scripts)
{
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById(objectId);

	for( auto currentScript = scripts.begin(); currentScript != scripts.end(); currentScript++ ) {
		std::string eventName = "";
		switch (type) //last_for_size handled in default
		{
		case EventType::EventClick :
			eventName = "OnClick";
			break;
		case EventType::EventTick :
			eventName = "OnTimer";
			break;
		default:
			assert(false); //In case we will add more functions
		}
		//Empty string left for ability to call different functions located in single script
		CScriptSolver solver(workingObject, *currentScript, eventName);

        std::shared_ptr<IDrawable> changedObject = solver.Run();   //Returns shared_ptr to changed object, but values already set in the scene
		assert( changedObject == workingObject );
	}
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
