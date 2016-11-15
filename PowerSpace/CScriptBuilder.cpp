#include <stdafx.h>
#include "CScriptBuilder.h"
#pragma warning( disable : 4100 )

CScriptBuilder::CScriptBuilder(std::shared_ptr<CScript> object)
{
	if (PyType_Ready(&engine_PythonScriptType) < 0) {
		pObject = nullptr;
	}
	else {
		pObject = std::shared_ptr<PyObject>(
			reinterpret_cast<PyObject *>(PyObject_NEW(engine_PythonScriptObject, &engine_PythonScriptType)),
			[=](PyObject *pObject)
		{
			PyObject_FREE(pObject);
		});
	}
}


int CScriptBuilder::PythonScript_onClick(engine_PythonScriptObject *self, void *closure)
{
	//TODO
}

int CScriptBuilder::PythonScript_onTimer(engine_PythonScriptObject *self, void *closure)
{
	//TODO
}

void CScriptBuilder::PythonScript_dealloc(engine_PythonScriptObject* self)
{
	Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

int CScriptBuilder::PythonScript_init(engine_PythonScriptObject *self, PyObject *args,
	PyObject *kwds)
{
/*	static char *kwlist[] = { "scriptType", NULL };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,
		&self->typeOfFunction)) 
	{
		return -1;
	} */ //Seems like we will be better without it
	return 0;
}

PyObject *CScriptBuilder::PythonScript_new(PyTypeObject *type, PyObject *args,
	PyObject *kwds)
{
	engine_PythonScriptObject *self;
	self = (engine_PythonScriptObject *)type->tp_alloc(type, 0);
	assert(self);
	return reinterpret_cast<PyObject *>(self);
}


PyMethodDef CScriptBuilder::PythonScript_methods[] = {
	{ "onClick", (PyCFunction)PythonScript_onClick, METH_VARARGS, "Action on click" },
	{ "onTimer", (PyCFunction)PythonScript_onTimer, METH_VARARGS, "Action on timer" },
	{ NULL }  /* Sentinel */
};


PyTypeObject CScriptBuilder::engine_PythonScriptObject = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Engine.CScript",             /* tp_name */
	sizeof(engine_PythonScriptObject), /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)PythonScript_dealloc,      /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_BASETYPE |
	Py_TPFLAGS_HAVE_GC,           /* tp_flags */
	"The wrapper for PowerSpace scripts",          /* tp_doc */
	0,							  /* tp_traverse */
	0,							  /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	PythonScript_methods,             /* tp_methods */
	0,                         /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)PythonScript_init,  /* tp_init */
	0,                         /*tp_alloc*/
	PythonScript_new              /*tp_new*/
};

PyObject *CScriptBuilder::GetRawpObjectRef() const
{
	return pObject.get();
}

std::shared_ptr<PyObject> CScriptBuilder::GetpObject() const
{
	return pObject;
}