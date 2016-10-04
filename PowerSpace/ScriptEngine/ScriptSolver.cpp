#include "stdafx.h"
#include "ScriptSolver.h"

CScriptSolver::CScriptSolver(std::shared_ptr<PyObject> object_, std::string path_,
	std::string func_) : object(object_), path(path_), func(func_) {}

std::shared_ptr<PyObject> CScriptSolver::Run() {
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
	Py_Initialize();
	pName = PyUnicode_FromString(path.c_str());

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != nullptr) {
		pFunc = PyObject_GetAttrString(pModule, path.c_str());
		if (pFunc && PyCallable_Check(pFunc)) {
			pValue = PyObject_CallObject(pFunc, object.get());
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	Py_Finalize();
	return std::make_shared<PyObject>(pValue);
}