#pragma once
#include <Python.h>
#include <Windows.h>
#include "structmember.h"
#include <Drawable.h>
#include <memory>

class CScriptBuilder
{
public:
	CScriptBuilder(const IdType& scriptName);

	typedef struct
	{
		PyObject_HEAD
			const int typeOfFunction; //0 for click 1 for timer
	} engine_PythonScriptObject;

	static void PythonScript_dealloc(engine_PythonScriptObject* self);

	static int PythonScript_init(engine_PythonScriptObject *self, PyObject *args,
		PyObject *kwds);

	static PyObject *PythonScript_new(PyTypeObject *type, PyObject *args,
		PyObject *kwds);

	static PyMethodDef PythonScript_methods[];

	static PyTypeObject engine_PythonScriptType;

	/*getters for the object*/

	PyObject *GetRawpObjectRef() const;

	std::shared_ptr<PyObject> GetpObject() const;

	static int PythonScript_onClick(engine_PythonScriptObject *self, void *closure);

	static int PythonScript_onTimer(engine_PythonScriptObject *self, void *closure);

private:
	std::shared_ptr<PyObject> pObject;
};

