#pragma once
#include <Python.h>
#include <unordered_map>

class ScriptHolder
{
	std::unordered_map<std::wstring, PyObject*> Scripts; //Scripts, that were run on this session

public:
	bool isScriptIn( std::wstring path );					 //Check is script in
	PyObject* getScript( std::wstring path );				 //Get script by path
	bool addScript( std::wstring path, PyObject* script ); //Add script that is not into the Holder yet
	int getSize() const;
};