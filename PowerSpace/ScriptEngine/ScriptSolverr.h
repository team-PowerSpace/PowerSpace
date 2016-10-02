#pragma once
#include <Python.h>

class CScriptSolver {
public:
	CScriptSolver(PyObject* object_, std::string path_, std::string func_);
	PyObject* Run();
private:
	PyObject* object;
	std::string path;
	std::string  func;
};