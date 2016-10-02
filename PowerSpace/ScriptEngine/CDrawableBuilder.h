#pragma once
#include <Python.h>
#include <Windows.h>
#include "structmember.h"
#include "Common\Drawable.h"
#include <memory>

class CDrawableBuilder {
public:
	CDrawableBuilder(std::shared_ptr<IDrawable> object);	
	PyObject* GetpObject();
private:
	PyObject* pObject;
};

typedef struct {
	PyObject_HEAD
	unsigned long int color;
	int xPos, yPos, width, height;
} CDrawableStruct;


static void CDrawableRedable(CDrawableStruct* self) {
	self->color = RGB(255, 0, 0);
}

static void CDrawableDealloc(CDrawableStruct* self) {
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int CDrawableInit(CDrawableStruct *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = { "color", "xPos", "yPos", "width", "heigh", NULL };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
		&self->color, &self->xPos, &self->yPos, &self->width, &self->height)) {	
		return -1;
	}	
	return 0;
}

static PyMemberDef CDrawableMembers[] = {
	{"color", T_ULONG, offsetof(CDrawableStruct, color), 0, "color" },
	{"xPos", T_INT, offsetof(CDrawableStruct, xPos), 0, "xPos of left top corner"},
	{"yPos", T_INT, offsetof(CDrawableStruct, yPos), 0, "yPos of left top corner" },
	{"width", T_INT, offsetof(CDrawableStruct, width), 0, "width of rect" },
	{"heigh", T_INT, offsetof(CDrawableStruct, height), 0, "heigh of rect" },
	{ NULL }  /* Sentinel */
};

static PyMethodDef CDrawableMethods[] = {
	{ "Redable", (PyCFunction)CDrawableRedable, METH_VARARGS, "Make the object red"},
	{ NULL }  /* Sentinel */
};

static PyTypeObject CDrawableType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"CDrawable",             /* tp_name */
	sizeof(CDrawableStruct), /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)CDrawableDealloc,      /* tp_dealloc */
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
	"CDrawable objects",          /* tp_doc */
	0,							  /* tp_traverse */
	0,							  /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	CDrawableMethods,             /* tp_methods */
	CDrawableMembers,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)CDrawableInit,      /* tp_init */
};