#include <stdafx.h>
#include <set>
#include "ScriptEngine.h"
#include <PyObjectBuilder.h>
#include <CDrawableBuilder.h>
#include "PyRunner.h"


CScriptEngine::CScriptEngine( CStage& _stage )
	: stage( _stage ), isPythonRunning( false )
{
	Py_Initialize(); //starting up Python right here
    isPythonRunning = true;
	LoadScene();
}

void CScriptEngine::LoadScene()
{
	PyObject* mainModule = PyImport_AddModule( "__main__" );
	globalDictionary = PyModule_GetDict( mainModule );
    localDictionary = PyDict_New();
    auto fromList = PyList_New( 0 );
    PyList_Append( fromList, PyUnicode_FromString( "*" ) );
    PyImport_ImportModuleEx( "Engine", globalDictionary, localDictionary, fromList );

    std::set<ScriptName> ScriptsToImport;
	std::unordered_map<IdType, IDrawablePtr>& objects = stage.GetObjects();

	for( auto obj : objects ) {
		auto scripts = obj.second->GetScripts();
		for( auto script : scripts ) {
			ScriptsToImport.insert(script.GetName());
		}			
	}
	for( auto script : ScriptsToImport ) {		
        //PyObject* module = PyUnicode_FromUnicode( script.c_str(), script.size() );
        //PyObject* module = PyUnicode_FromString( std::string(script.begin(), script.end()).c_str() );

        auto tmp = std::wstring( L"import " ) + script;
        std::string constructorScriptASCII = std::string( tmp.begin(), tmp.end() );
        //PyObject* localDictionary = PyDict_New();
        //PyObject* result = PyRun_String( constructorScriptASCII.c_str(), Py_file_input, globalDictionary, localDictionary );
        //assert( result != 0 );
		//auto pModule = PyImport_Import( module );
        PyRun_SimpleString(constructorScriptASCII.c_str());
        
        auto moduleName = std::string( script.begin(), script.end() ).c_str();
        PyImport_ImportModuleEx( moduleName, globalDictionary, localDictionary, fromList);
        /*auto res1 = PyObject_HasAttrString( pModule, "class_lights" );
        assert( res1 != 0 );
        assert( pModule != 0 );*/
	}
	
	AddPyScripts();
	for( std::pair<IdType, IDrawablePtr> object : objects ) {		
		AddPyObject(object.first, object.second);
	}
}

void CScriptEngine::AddPyScripts()
{
	std::unordered_map<IdType, IDrawablePtr>& objects = stage.GetObjects();

	for( auto obj : objects ) {
		auto scripts = obj.second->GetScripts();
		for( auto script : scripts ) {
			std::wstring constructorScript = obj.first;
			constructorScript.append( L"_" );
			constructorScript.append( script.GetName() );
			constructorScript.append( L" = " );
            constructorScript.append( script.GetName() );
            constructorScript.append( L"." );
            constructorScript.append( script.GetName() );
            constructorScript.append( L"()" );
			std::string constructorScriptASCII = std::string( constructorScript.begin(), constructorScript.end() );

			PyObject* result = PyRun_String( constructorScriptASCII.c_str(), Py_file_input, globalDictionary, localDictionary );
			if( !result ) {
				throw "Error: invalid input";
			}
		}
	}
}

void CScriptEngine::AddPyObject( IdType name, IDrawablePtr description )
{
	std::string constructorScript( name.begin(), name.end() );
	PyObject* pyObjName = PyUnicode_FromString( constructorScript.c_str() );
	constructorScript.append( "=Engine.CDrawable()" );
	PyObject* result = PyRun_String( constructorScript.c_str(), Py_file_input, globalDictionary, localDictionary );
	if( !result ) {
		throw "Error: invalid input";		
	}
	PyDict_Update( globalDictionary, localDictionary );
	if( PyDict_Contains( localDictionary, pyObjName ) ) {		
		PyObject* pyVal = PyDict_GetItem( localDictionary, pyObjName );		
		pyScene.insert( std::pair<IdType, std::shared_ptr<CDrawableBuilder>>( name, 
			std::make_shared<CDrawableBuilder>( pyVal, description ) ));
	} else {
		throw "Error: failed get object";
	}
}

 void CScriptEngine::RunScripts( const IdType& objectId, EventType type, const std::vector<IdType>& scripts )
{
	PyRunner::run(objectId, type, scripts);
	std::shared_ptr<CDrawableBuilder> pObject = pyScene.at( objectId );
    std::shared_ptr<IDrawable> object = stage.GetObjectById( objectId );
    PyRunner::update( pObject, object );
}
