#pragma once
#include <Python.h>
#include <string>
#include <memory>
#include <map>

#include "Drawable.h"
#include "CDrawableBuilder.h"
#include "CScriptHolder.h"

class CScriptSolver
{
public:
	CScriptSolver( std::shared_ptr<IDrawable> object_, std::wstring path_, std::string func_, std::shared_ptr<ScriptHolder> holder );
	void FillDict( PyObject* dict );
	void UpdateDict( PyObject* dict );
	std::shared_ptr<IDrawable> Run();
	std::shared_ptr<IDrawable> RunWithDict();

	std::shared_ptr<ScriptHolder> holder;
private:
	std::map<std::string, long> members;
	std::wstring scriptName;
	std::string  func;
	std::shared_ptr<IDrawable> object;
	std::shared_ptr<CDrawableBuilder> pObject;

	PyObject *GetPyFunction( PyObject *pModule ) const;
	void UpdateObject();
};