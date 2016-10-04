#pragma once
#include <Python.h>
#include <string>
#include <memory>

class CScriptSolver {
public:
	CScriptSolver(std::shared_ptr<PyObject> object_, std::string path_, std::string func_);
	std::shared_ptr<PyObject> Run();
private:
	std::shared_ptr<PyObject> object;
	std::string path;
	std::string  func;
};