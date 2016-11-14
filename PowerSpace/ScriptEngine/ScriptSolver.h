#pragma once
#include <Python.h>
#include <string>
#include <memory>
#include <map>
#include <CScriptHolder.h>
#include <Drawable.h>
#include <CDrawableBuilder.h>
#include <CScriptHolder.h>

class CScriptSolver
{
public:
	CScriptSolver( std::shared_ptr<IDrawable> object_, PyObject* pyObject_, std::string func_);
	std::shared_ptr<IDrawable> Run();

	std::shared_ptr<ScriptHolder> holder;
private:
	PyObject* pyObject;
	std::string  func;
	std::shared_ptr<IDrawable> object;
	std::shared_ptr<CDrawableBuilder> pObject;

	PyObject *GetPyFunction(PyObject *pModule) const;
	void UpdateObject();
};