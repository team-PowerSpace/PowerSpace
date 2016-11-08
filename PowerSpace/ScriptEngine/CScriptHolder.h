#pragma once
#include <Python.h>
#include <unordered_map>
#include <Script.h>
#include <codecvt>
#include <Exception>
#include <iostream>
#include <fstream>


class ScriptHolder
{
	//key -> pair of <objId, typeOfScript>
	//value -> all scripts of type associated with the object
	std::unordered_map<std::pair<std::string, EventType>, std::vector<PyObject*>> Scripts; //Scripts, that were run on this session

public:
	//not sure if we will need it later
	//bool isScriptIn( std::pair<int, std::string>);					 //Check is script in
	std::vector<PyObject*> getScript(std::pair<std::string, EventType> key);				 //Get script by path
	bool addScript(std::pair<std::string, EventType> key, PyObject* script );   //Add script that is not into the Holder yet
	bool addScript(std::pair<std::string, EventType> key, CScript script);		//Add script by it's CScript
	void removeScripts(std::string objectId);	//Remove scripts when object removed

	int getSize() const;
	void decAllRefs();
	

	//TODO: Is needed to solve link error 2005 with script engine.obj
	//~ScriptHolder();                                         //We need to free refs to modules
};