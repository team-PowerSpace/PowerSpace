﻿#include <stdafx.h>
#include <set>
#include "ScriptEngine.h"
#include <PyObjectBuilder.h>
#include <CDrawableBuilder.h>
#include "PyRunner.h"
#include <Windows.h>

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
	try {
		ImportModules();
	}
	catch( std::string& e ) {
		MessageBoxA( NULL, e.c_str(), "Error", NULL );
	}
	localDictionary = PyDict_New();
	globalDictionary = PyModule_GetDict( mainModule );
	try {
		AddPyScripts();
	}
	catch( std::string& e ) {
		MessageBoxA( NULL, e.c_str(), "Error", NULL );
	}

	std::unordered_map<IdType, IDrawablePtr>& objects = stage.GetObjects();
	try {
		for( std::pair<IdType, IDrawablePtr> object : objects ) {
			AddPyObject( object.first, object.second );
		}
	}
	catch( std::string& e ) {
		MessageBoxA( NULL, e.c_str(), "Error", NULL );
	}
	MessageBoxA( NULL, "LoadScene succeded" , "OK", NULL );
}

void CScriptEngine::ImportModules()
{
	int result = 0;
	result = PyRun_SimpleString( "from PythonDrawable import Drawable" );
	if( result != 0 ) {
		throw "Can't import PythonDrawable module";
	}
	std::set<ScriptName> ScriptsToImport;
	std::unordered_map<IdType, IDrawablePtr>& objects = stage.GetObjects();

	for( auto obj : objects ) {
		auto scripts = obj.second->GetScripts();
		for( auto script : scripts ) {
			ScriptsToImport.insert( script.GetName() );
		}
	}
	for( auto script : ScriptsToImport ) {		
		std::string scriptName = std::string( script.begin(), script.end() );
		std::string importCommand = std::string( "from " ) + scriptName + std::string( " import " ) + scriptName;
		result =  PyRun_SimpleString( importCommand.c_str() );
		if( result != 0 ) {
			throw "Can't import " + scriptName + " module";
		}		
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
            constructorScript.append( L"()" );
			std::string constructorScriptASCII = std::string( constructorScript.begin(), constructorScript.end() );
			
			PyObject* result = PyRun_String( constructorScriptASCII.c_str(), Py_file_input, globalDictionary, localDictionary );
			if( !result ) {
				throw "Invalid input: " + constructorScriptASCII;
			}
		}
	}
}

void CScriptEngine::AddPyObject( IdType name, IDrawablePtr description )
{
	std::string constructorScript( name.begin(), name.end() );
	PyObject* pyObjName = PyUnicode_FromString( constructorScript.c_str() );
	constructorScript.append( "=Drawable()" );
	PyObject* result = PyRun_String( constructorScript.c_str(), Py_file_input, globalDictionary, localDictionary );
	if( !result ) {
		throw "Invalid input: " + constructorScript;
	}
	PyDict_Update( globalDictionary, localDictionary );
	if( PyDict_Contains( localDictionary, pyObjName ) ) {		
		PyObject* pyVal = PyDict_GetItem( localDictionary, pyObjName );	
		if( !pyVal ) {
			throw "Failed get object from dictianary: " + std::string( name.begin(), name.end() );
		}
		try {
			pyScene.insert( std::pair<IdType, std::shared_ptr<CDrawableBuilder>>( name,
				std::make_shared<CDrawableBuilder>( pyVal, description ) ) );
		}
		catch( ... ) {		
			throw "Failed inset object to scene: " + std::string( name.begin(), name.end() );
		}
	}		else {
		throw "Failed get object: " + std::string( name.begin(), name.end());
	}
}

 void CScriptEngine::RunScripts( const IdType& objectId, EventType type, const std::vector<IdType>& scripts )
{
	MessageBoxW( NULL, objectId.c_str(), L"Error", NULL );
	try {
		PyRunner::run( objectId, type, scripts );
	}
	catch( std::string& e ) {
		MessageBoxA( NULL, e.c_str(), "Error", NULL );
	}
	std::shared_ptr<CDrawableBuilder> pObject = pyScene.at( objectId );
    std::shared_ptr<IDrawable> object = stage.GetObjectById( objectId );
    PyRunner::update( pObject, object );
}
