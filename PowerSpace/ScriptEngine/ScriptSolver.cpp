#include <stdafx.h>
#include "ScriptSolver.h"
#include <iostream>
#include <exception>


CScriptSolver::CScriptSolver(std::shared_ptr<IDrawable> obj, std::wstring path_,
	std::string func_, std::shared_ptr<ScriptHolder> holder_) : path(path_), func(func_), object(obj), holder(holder_) {
	std::pair<std::string, long> xPos("x", obj->GetContainingBox().left);
	std::pair<std::string, long> yPos("y", obj->GetContainingBox().top);
	std::pair<std::string, long> width("width", obj->GetContainingBox().right - obj->GetContainingBox().left);
	std::pair<std::string, long> heigth("heigth", obj->GetContainingBox().bottom - obj->GetContainingBox().top);
	std::pair<std::string, long> color("color", obj->GetColor());
	members.insert(xPos);
	members.insert(yPos);
	members.insert(width);
	members.insert(heigth);
	members.insert(color);

}

std::shared_ptr<IDrawable> CScriptSolver::Run() {
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
		
	pName = nullptr;
	
	if (!holder->isScriptIn(path))
	{
		pName = PyUnicode_FromUnicode(path.c_str(), path.size());
		pModule = PyImport_Import(pName);
		holder->addScript(path, pModule);
	} else {
		pModule = holder->getScript(path);
	}
	Py_DECREF(pName);

	if (pModule != nullptr) {
		std::string funcName1 = "OnClick";
		std::string funcName2 = "OnTimer";
		//Lower lies decent line of code, which extracts the function we seek from script.
		//It is not used for now, due to the fact that we only have two types of functions
		//and they both can not be ever found in the same script file. If needed this line can be
		//used later. The lines 39-52 are the temporary solution, which, of course, can be written
		//more carefully, but if it would look better it might in the end become final
		//and not pretty solution, even if we will have to handle way more types of functions)
		//pFunc = PyObject_GetAttrString(pModule, func.c_str());
		pFunc = PyObject_GetAttrString(pModule, funcName1.c_str());
		if (!(pFunc && PyCallable_Check(pFunc)))
		{
			pFunc = PyObject_GetAttrString(pModule, funcName2.c_str());
		}
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			pDict = PyDict_New();
			FillDict(pDict);
			PyTuple_SetItem(pArgs, 0, pDict);
			pValue = PyObject_CallObject(pFunc, pArgs);
			UpdateDict(pDict);
			Py_XDECREF(pValue);
		}

		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	object->SetColor(members.find("color")->second);
	RECT rect;
	rect.top = members.find("y")->second;
	rect.left = members.find("x")->second;
	rect.bottom = rect.top + members.find("heigth")->second;
	rect.right = rect.left + members.find("width")->second;
	object->SetContainingBox(rect);
	return object;
}

void CScriptSolver::FillDict(PyObject* dict) {
	for (auto member : members) {
		PyDict_SetItem(dict, Py_BuildValue("s", member.first.c_str()),
			Py_BuildValue("l", member.second));
	}
}

void CScriptSolver::UpdateDict(PyObject* dict) {
	PyObject* keys = PyDict_Keys(dict);
	members.clear();
	for (int i = 0; i < PyList_Size(keys); i++) {
		PyObject* key = PyList_GetItem(keys, i);
		PyObject* val = PyDict_GetItem(dict, key);
		std::pair<std::string, long> pair(std::string(PyUnicode_AsUTF8(key)),
			PyLong_AsLong(val));
		members.insert(pair);
	}
}