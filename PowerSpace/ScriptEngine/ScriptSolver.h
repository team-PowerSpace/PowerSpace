#pragma once
#include <Python.h>
#include <string>
#include <memory>
#include <map>

#include "Drawable.h"

class CScriptSolver {
public:
	CScriptSolver(std::shared_ptr<IDrawable> object_, std::wstring path_, std::string func_);
	void FillDict(PyObject* dict);
	void UpdateDict(PyObject* dict);
	std::shared_ptr<IDrawable> Run();
private:
	std::map<std::string, long> members;
	std::wstring path;
	std::string  func;
	std::shared_ptr<IDrawable> object;
};