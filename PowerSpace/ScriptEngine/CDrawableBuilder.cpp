#include <stdafx.h>
#include "CDrawableBuilder.h"

CDrawableBuilder::CDrawableBuilder( std::shared_ptr<IDrawable> object )
{
	if( PyType_Ready( &CDrawableType ) < 0 ) {
		pObject = nullptr;
	} else {
		RECT rect = object->GetContainingBox();
		pObject = std::shared_ptr<PyObject>(
			reinterpret_cast<PyObject*>(PyObject_NEW(CDrawableStruct, &CDrawableType)), 
			[=] ( PyObject *pObject )
		{
			PyObject_FREE(pObject);
		});

		PyObject* args = PyTuple_New( 5 );
		PyTuple_SetItem( args, 0, PyLong_FromUnsignedLong( object->GetColor() ) );
		PyTuple_SetItem( args, 1, PyLong_FromLong( rect.left ) );
		PyTuple_SetItem( args, 2, PyLong_FromLong( rect.top ) );
		PyTuple_SetItem( args, 3, PyLong_FromLong( rect.right - rect.left ) );
		PyTuple_SetItem( args, 4, PyLong_FromLong( rect.bottom - rect.top ) );

		PyObject* kwds = PyTuple_New( 5 );
		PyTuple_SetItem( kwds, 0, PyUnicode_FromString( "color" ) );
		PyTuple_SetItem( kwds, 1, PyUnicode_FromString( "xPos" ) );
		PyTuple_SetItem( kwds, 2, PyUnicode_FromString( "yPos" ) );
		PyTuple_SetItem( kwds, 3, PyUnicode_FromString( "width" ) );
		PyTuple_SetItem( kwds, 4, PyUnicode_FromString( "height" ) );

		pObject->ob_type->tp_init( pObject.get(), args, kwds );
		Py_XDECREF( args );
		Py_XDECREF( kwds );

		int color, xPos, yPos, width, height;

		if (PyArg_ParseTuple(pObject.get(), "liiii:decoder", &color, &xPos, &yPos, &width, &height) != 0) {
			::MessageBox(0, L"lol", L"lol", MB_YESNO);
		}
	}
}


void CDrawableBuilder::CDrawableRedable( CDrawableStruct* self )
{
	self->color = RGB( 255, 0, 0 );
}

void CDrawableBuilder::CDrawableDealloc( CDrawableStruct* self )
{
	Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>(self) );
}

int CDrawableBuilder::CDrawableInit( CDrawableStruct *self, PyObject *args,
	PyObject *kwds )
{
	static char *kwlist[] = { "color", "xPos", "yPos", "width", "height", NULL };
	if( !PyArg_ParseTupleAndKeywords( args, kwds, "|liiii", kwlist,
		&self->color, &self->xPos, &self->yPos, &self->width, &self->height ) ) {
		return -1;
	}
	return 0;
}

PyMemberDef CDrawableBuilder::CDrawableMembers[] = {
	{ "color", T_ULONG, offsetof( CDrawableStruct, color ), 0, "color" },
	{ "xPos", T_INT, offsetof( CDrawableStruct, xPos ), 0, "xPos of left top corner" },
	{ "yPos", T_INT, offsetof( CDrawableStruct, yPos ), 0, "yPos of left top corner" },
	{ "width", T_INT, offsetof( CDrawableStruct, width ), 0, "width of rect" },
	{ "height", T_INT, offsetof( CDrawableStruct, height ), 0, "height of rect" },
	{ NULL }  /* Sentinel */
};

PyMethodDef CDrawableBuilder::CDrawableMethods[] = {
	{ "Redable", (PyCFunction)CDrawableRedable, METH_VARARGS, "Make the object red" },
	{ NULL }  /* Sentinel */
};

PyTypeObject CDrawableBuilder::CDrawableType = {
	PyVarObject_HEAD_INIT( NULL, 0 )
	"CDrawable",             /* tp_name */
	sizeof( CDrawableStruct ), /* tp_basicsize */
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

std::shared_ptr<PyObject> CDrawableBuilder::GetpObject()
{
	return pObject;
}