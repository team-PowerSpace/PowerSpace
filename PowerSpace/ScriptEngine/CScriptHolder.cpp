#include <stdafx.h>

#include "CScriptHolder.h"


bool ScriptHolder::addScript(std::pair<IdType, EventType> key, PyObject* script )
{
	//For now scripts hotfixes in running Viewer are not supported
	Py_INCREF( script );
	if (Scripts.find(key) != Scripts.end())
	{
		Scripts.find(key)->second.push_back(script);
		return true;
	}
	else 
	{
		std::vector<PyObject*> newVector;
		newVector.push_back(script);
		std::pair<std::pair<IdType, EventType>, std::vector<PyObject*>> toInsert(key, newVector);
		Scripts.insert(toInsert);
		return true;

	}
	
	return false;
}

void ScriptHolder::removeScripts(std::string objectId)
{
	std::vector<EventType> types;
	types.push_back(EventType::EventClick);
	types.push_back(EventType::EventTick);
	assert(types.size() == (size_t) EventType::ALWAYS_LAST_FOR_SIZE);
	for (auto type : types)
	{
		Scripts.erase(std::pair<IdType, EventType>(objectId, type));
	}
}

bool ScriptHolder::addScript(std::pair<IdType, EventType> key, CScript script)
{
	TPath wstrPath(script.GetPath());
	std::ifstream stream(wstrPath.data(), std::ifstream::in); //The best way to check path validity is trying to open it
	if (!stream.good()) {
		stream.close();
		std::cout << "The file doesn't exist" << std::endl;
		assert(false);
	}
	stream.close();

	//Deleteing the path and saving only script's filename (with extention)
	std::wstring scriptName = wstrPath.substr(wstrPath.find_last_of(L"\\/") + 1);

	//Deleting the extention of script
	std::wstring scriptNameWithoutExtention = scriptName.substr(0, scriptName.find(L"."));
	//For now scripts hotfixes in running Viewer are not supported
	PyObject* pName;
	pName = PyUnicode_FromUnicode(scriptNameWithoutExtention.c_str(), scriptNameWithoutExtention.size());
	if (Scripts.find(key) != Scripts.end())
	{
		Scripts.find(key)->second.push_back(pName);
		return true;
	}
	else
	{
		std::vector<PyObject*> newVector;
		newVector.push_back(pName);
		std::pair<std::pair<IdType, EventType>, std::vector<PyObject*>> toInsert(key, newVector);
		Scripts.insert(toInsert);
		return true;

	}

	//Returns true if was inserted succefully
}

size_t ScriptHolder::getSize() const
{
    return Scripts.size();
}

std::vector<PyObject*> ScriptHolder::getScript(std::pair<IdType, EventType> key)
{
	std::vector<PyObject*> objects;
	try {
		objects = Scripts.at( key );		//Is okay if operation succeful
	} catch( std::out_of_range e ) {

		return std::vector<PyObject*>();
	}
	return objects;
}

void ScriptHolder::decAllRefs()
{
	for (auto iter : Scripts)
	{
		for (auto script : iter.second)
		{
			Py_DecRef(script);
		}
	}
}

/*bool ScriptHolder::isScriptIn( std::wstring path )
{
	auto iterator = Scripts.find( path );
	if( iterator == Scripts.end() ) {
		return false;		 //If points to the .end() then gg
	}
	return true;			 //If you want to support hotfixes to scripts -- don't forget to update code here
}*/

void ScriptHolder::decAllRefsAndClearObjects()
{
    for( auto a = Scripts.begin(); a != Scripts.end(); a++ ) {
        PyObject* toDecRef = a->second;
        Py_XDECREF( toDecRef );
        Py_CLEAR( toDecRef );
        Scripts.erase( a );
    }
}
//TODO: Need to solve link error with scriptEngine.obj
/*
ScriptHolder::~ScriptHolder()
{
    for (auto iterator : Scripts) {
    Py_DECREF(iterator.second);
    }
}
*/