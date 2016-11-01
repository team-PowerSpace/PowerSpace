#pragma once
#include <Python.h>
#include <unordered_map>

class ScriptHolder
{
	std::unordered_map<std::wstring, PyObject*> Scripts; //Scripts, that were run on this session

public:
	bool isScriptIn( std::wstring path );					 //Check is script in
	PyObject* getScript( std::wstring path );				 //Get script by path
	bool addScript( std::wstring path, PyObject* script );   //Add script that is not into the Holder yet
	size_t getSize() const;
	void decAllRefsAndClearObjects();
	//TODO: Is needed to solve link error 2005 with script engine.obj
	//~ScriptHolder();                                         //We need to free refs to modules
};