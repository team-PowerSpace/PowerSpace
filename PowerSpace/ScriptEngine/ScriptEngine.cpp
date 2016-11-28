#include <stdafx.h>

#include "ScriptEngine.h"
#include <ScriptSolver.h>
#include <PyObjectBuilder.h>
#include <CDrawableBuilder.h>



CScriptEngine::CScriptEngine( CStage& _stage )
    : stage( _stage ), isPythonRunning( false )
{
    //LoadScene();
}

void CScriptEngine::LoadScene()
{
    std::unordered_map<IdType, IDrawablePtr>& objects = stage.GetObjects();
    PyObject* mainModule = PyImport_AddModule( "__main__" );
    globalDictionary = PyModule_GetDict( mainModule );
    for( std::pair<IdType, IDrawablePtr> object : objects ) {        
        AddPyObject(object.first, object.second);
    }
}

void CScriptEngine::AddPyObject( IdType name, IDrawablePtr description )
{
    std::string constructorScript( name.begin(), name.end() );
    PyObject* pyObjName = PyUnicode_FromString( constructorScript.c_str() );
    constructorScript.append( "=Engine.CDrawable()" );
    PyObject* localDictionary = PyDict_New();
    PyObject* result = PyRun_String( constructorScript.c_str(), Py_file_input, globalDictionary, localDictionary );
    if( !result ) {
        throw "Error: invalid input";        
    }
    PyDict_Update( globalDictionary, localDictionary );
    if( PyDict_Contains( localDictionary, pyObjName ) ) {
        PyObject* pyVal = PyDict_GetItem( localDictionary, pyObjName );
        pyScene.insert( std::pair<IdType, PyObject*>( name, pyVal ));
    } else {
        throw "Error: failed get object";
    }
}

std::vector<int> CScriptEngine::RunScripts( IdType objectId, EventType type, std::vector<PyObject*> scripts )
{
    std::shared_ptr<IDrawable> workingObject = stage.GetObjectById( objectId );
    /*auto it = pyScene.find( objectId );
    if( it == pyScene.end() ) {
        throw "Can't find object in scene";
    }
    PyObject* sceneObject = it->second;*/
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
        CScriptSolver solver(workingObject, /*sceneObject,*/ *currentScript, eventName);
        
        std::shared_ptr<IDrawable> changedObject = solver.Run();   //Returns shared_ptr to changed object, but values already set in the scene
        assert( changedObject == workingObject );
    }
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
