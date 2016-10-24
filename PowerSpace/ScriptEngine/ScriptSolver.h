#pragma once
#include <Python.h>
#include <string>
#include <memory>
#include <map>
#include "CScriptHolder.h"
#include "Drawable.h"

class CScriptSolver {
public:
	CScriptSolver(std::shared_ptr<IDrawable> object_, std::wstring path_, std::string func_,
		std::shared_ptr<ScriptHolder> holder);
	void FillDict(PyObject* dict);
	void UpdateDict(PyObject* dict);
	std::shared_ptr<IDrawable> Run();
	std::shared_ptr<ScriptHolder> holder;
private:
	std::map<std::string, long> members;
	std::wstring path;
	std::string  func;
	std::shared_ptr<IDrawable> object;
};