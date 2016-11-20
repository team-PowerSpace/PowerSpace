#include <stdafx.h>

#include "ScriptEngine.h"
#include <ScriptSolver.h>
#include <PyObjectBuilder.h>
#include <CDrawableBuilder.h>
#include "PyRunner.h"


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

 void CScriptEngine::RunScripts( const IdType& objectId, EventType type, const std::vector<IdType>& scripts )
{
	PyRunner::run(objectId, type, scripts);
    std::shared_ptr<CDrawableBuilder> pObject;// = getFromHolder();
    std::shared_ptr<IDrawable> object = stage.GetObjectById( objectId );
    PyRunner::update( pObject, object );
}
