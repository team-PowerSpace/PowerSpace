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
	CScriptSolver( std::shared_ptr<IDrawable> object_,/* PyObject* sceneObject,*/ PyObject* module_, std::string func_);
	
	std::shared_ptr<IDrawable> Run();

	std::shared_ptr<ScriptHolder> holder;
private:
	PyObject* module;
	std::string  func;
	std::shared_ptr<IDrawable> object;
	std::shared_ptr<CDrawableBuilder> pObject;
	/*PyObject* pObject;*/
	PyObject *GetPyFunction(PyObject *pModule) const;
	void UpdateObject();
};