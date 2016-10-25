#include <stdafx.h>

#include "CScriptHolder.h"

bool ScriptHolder::addScript( std::wstring path, PyObject* script )
{
	std::pair<std::wstring, PyObject*> toInsert( path, script );  //For now scripts hotfixes in running Viewer are not supported
	Py_INCREF(script);
	return (Scripts.insert( toInsert )).second;					//Returns true if was inserted succefully
}

int ScriptHolder::getSize() const
{
	return Scripts.size();
}

PyObject* ScriptHolder::getScript( std::wstring path )
{
	PyObject* obj;
	try {
		obj = Scripts.at( path );		//Is okay if operation succeful
	} catch( std::out_of_range e ) {
		return nullptr;
	}
	Py_INCREF(obj);
	return obj;
}

bool ScriptHolder::isScriptIn( std::wstring path )
{
	auto iterator = Scripts.find( path );
	if( iterator == Scripts.end() ) {
		return false;		 //If points to the .end() then gg
	}
	return true;			 //If you want to support hotfixes to scripts -- don't forget to update code here
}
//Need to solve link error with scriptEngine.obj
/*
ScriptHolder::~ScriptHolder()
{
	for (auto iterator : Scripts) {
		Py_DECREF(iterator.second);
	}
}
*/
