#include <stdafx.h>
#include "CTextBoxBuilder.h"
#pragma warning( disable : 4100 )

CTextBoxBuilder::CTextBoxBuilder( std::shared_ptr<IDrawable> object )
{
	if( PyType_Ready( &engine_PythonTextBoxType ) < 0 ) {
		pObject = nullptr;
	} else {		
		pObject = std::shared_ptr<PyObject>(
			reinterpret_cast<PyObject *>(PyObject_NEW( engine_PythonTextBoxObject, &engine_PythonTextBoxType )),
			[=]( PyObject *pObject )
		{
			PyObject_FREE( pObject );
		} );
		std::shared_ptr<CTextBoxObject> castedObject = std::dynamic_pointer_cast<CTextBoxObject>(object);
		PythonTextBox_set_text(
			reinterpret_cast<engine_PythonTextBoxObject *>(pObject.get()), castedObject->GetText(), 0 );
		PythonTextBox_set_fontSize(
			reinterpret_cast<engine_PythonTextBoxObject *>(pObject.get()), castedObject->GetText(), 0 );
		PythonTextBox_set_fontColor(
			reinterpret_cast<engine_PythonTextBoxObject *>(pObject.get()), castedObject->GetText(), 0 );
	}
}

void CTextBoxBuilder::PythonTextBox_dealloc( engine_PythonTextBoxObject* self )
{
	Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>(self) );
}

int CTextBoxBuilder::PythonTextBox_init( engine_PythonTextBoxObject *self, PyObject *args,
	PyObject *kwds )
{
	static char *kwlist[] = { "text", "fontSize", "fontColor", NULL };
	if( !PyArg_ParseTupleAndKeywords( args, kwds, "|liiii", kwlist,
		&self->text, &self->fontSize, &self->fontColor ) ) {
		return -1;
	}
	return 0;
}

PyObject *CTextBoxBuilder::PythonTextBox_new( PyTypeObject *type, PyObject *args,
	PyObject *kwds )
{
	engine_PythonTextBoxObject *self;
	self = (engine_PythonTextBoxObject *)type->tp_alloc( type, 0 );
	if( self ) {
		self->text = "Text";
		self->fontSize = 12;
		self->fontColor = 0;		
	}
	return reinterpret_cast<PyObject *>(self);
}

PyMemberDef CTextBoxBuilder::PythonTextBox_members[] = {
	{ "text", T_STRING, offsetof( engine_PythonTextBoxObject, text ), 0, "text" },
	{ "fontSize", T_UINT, offsetof( engine_PythonTextBoxObject, fontSize ), 0, "font size" },
	{ "fontColor", T_ULONG, offsetof( engine_PythonTextBoxObject, fontColor ), 0, "font color" },
	{ NULL }  /* Sentinel */
};

PyMethodDef CTextBoxBuilder::PythonTextBox_methods[] = {
	{ NULL }  /* Sentinel */
};

PyGetSetDef CTextBoxBuilder::PythonTextBox_getseters[] = {
	{ "text",
	(getter)PythonTextBox_get_text, (setter)PythonTextBox_set_text,
	"text",
	NULL },
	{ "fontSize",
	(getter)PythonTextBox_get_fontSize, (setter)PythonTextBox_set_fontSize,
	"font size",
	NULL },
	{ "fontColor",
	(getter)PythonTextBox_get_fontColor, (setter)PythonTextBox_set_fontColor,
	"font color",
	NULL },	
	{ NULL }  /* Sentinel */
};

PyTypeObject CTextBoxBuilder::engine_PythonTextBoxType = {
	PyVarObject_HEAD_INIT( NULL, 0 )
	"Engine.CTextBox",             /* tp_name */
	sizeof( engine_PythonTextBoxObject ), /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)PythonTextBox_dealloc,      /* tp_dealloc */
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
	"The wrapper for PowerSpace objects",          /* tp_doc */
	0,							  /* tp_traverse */
	0,							  /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	PythonTextBox_methods,             /* tp_methods */
	PythonTextBox_members,             /* tp_members */
	PythonTextBox_getseters,                         /* tp_getset */
	/*CDrawableObject*/0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)PythonTextBox_init,  /* tp_init */
	0,                         /*tp_alloc*/
	PythonTextBox_new              /*tp_new*/
};


PyObject *CTextBoxBuilder::GetRawpObjectRef() const
{
	return pObject.get();
}

std::shared_ptr<PyObject> CTextBoxBuilder::GetpObject() const
{
	return pObject;
}

char* CTextBoxBuilder::PythonTextBox_get_text( engine_PythonTextBoxObject *self, void *closure )
{
	return self->text;
}

unsigned int CTextBoxBuilder::PythonTextBox_get_fontSize( engine_PythonTextBoxObject *self, void *closure )
{
	return self->fontSize;
}

unsigned long int CTextBoxBuilder::PythonTextBox_get_fontColor( engine_PythonTextBoxObject *self, void *closure )
{
	return self->fontColor;
}

/*setters for the object*/
int CTextBoxBuilder::PythonTextBox_set_text( engine_PythonTextBoxObject *self, char* value, void *closure )
{
	self->text = value;
	return 0;
}

int CTextBoxBuilder::PythonTextBox_set_fontSize( engine_PythonTextBoxObject *self, unsigned int value, void *closure )
{
	self->fontSize = value;
	return 0;
}

int CTextBoxBuilder::PythonTextBox_set_fontColor( engine_PythonTextBoxObject *self, unsigned long int value, void *closure )
{
	self->fontColor = value;
	return 0;
}
