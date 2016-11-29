#include <stdafx.h>
#include "CDrawableBuilder.h"
#pragma warning( disable : 4100 )

CDrawableBuilder::CDrawableBuilder( std::shared_ptr<IDrawable> object )
{
    if( PyType_Ready( &engine_PythonDrawableType ) < 0 ) {
        pObject = nullptr;
    } else {
        RECT rect = object->GetContainingBox();
        pObject = std::shared_ptr<PyObject>(
            reinterpret_cast<PyObject *>(PyObject_NEW( engine_PythonDrawableObject, &engine_PythonDrawableType )),
            [=]( PyObject *pObject ) {
            PyObject_FREE( pObject );
        } );

        PythonDrawable_set_color(
            reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), object->GetColor(), 0 );
        PythonDrawable_set_width(
            reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.right - rect.left, 0 );
        PythonDrawable_set_height(
            reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.bottom - rect.top, 0 );
        PythonDrawable_set_xPos(
            reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.left, 0 );
        PythonDrawable_set_yPos(
            reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.top, 0 );
    }
}

CDrawableBuilder::CDrawableBuilder( PyObject* object, std::shared_ptr<IDrawable> description )
{
    RECT rect = description->GetContainingBox();
    pObject = std::shared_ptr<PyObject>(
        reinterpret_cast<PyObject *>(object),
        [=]( PyObject *pObject ) {
        PyObject_FREE( pObject );
    } );
    PythonDrawable_set_color(
        reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), description->GetColor(), 0 );
    PythonDrawable_set_width(
        reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.right - rect.left, 0 );
    PythonDrawable_set_height(
        reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.bottom - rect.top, 0 );
    PythonDrawable_set_xPos(
        reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.left, 0 );
    PythonDrawable_set_yPos(
        reinterpret_cast<engine_PythonDrawableObject *>(pObject.get()), rect.top, 0 );
}

void CDrawableBuilder::PythonDrawable_makeRed( engine_PythonDrawableObject* self )
{
    self->color = RGB( 255, 0, 0 );
}

void CDrawableBuilder::PythonDrawable_dealloc( engine_PythonDrawableObject* self )
{
    Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>(self) );
}

int CDrawableBuilder::PythonDrawable_init( engine_PythonDrawableObject *self, PyObject *args,
    PyObject *kwds )
{
    static char *kwlist[] = { "color", "xPos", "yPos", "width", "height", NULL };
    if( !PyArg_ParseTupleAndKeywords( args, kwds, "|liiii", kwlist,
        &self->color, &self->xPos, &self->yPos, &self->width, &self->height ) ) {
        return -1;
    }
    return 0;
}

PyObject *CDrawableBuilder::PythonDrawable_new( PyTypeObject *type, PyObject *args,
    PyObject *kwds )
{
    engine_PythonDrawableObject *self;
    self = (engine_PythonDrawableObject *)type->tp_alloc( type, 0 );
    if( self ) {
        self->color = 255;
        self->height = 50;
        self->width = 100;
        self->xPos = 0;
        self->yPos = 0;
    }
    return reinterpret_cast<PyObject *>(self);
}

PyMemberDef CDrawableBuilder::PythonDrawable_members[] = {
    { "color", T_ULONG, offsetof( engine_PythonDrawableObject, color ), 0, "color" },
    { "xPos", T_INT, offsetof( engine_PythonDrawableObject, xPos ), 0, "xPos of left top corner" },
    { "yPos", T_INT, offsetof( engine_PythonDrawableObject, yPos ), 0, "yPos of left top corner" },
    { "width", T_INT, offsetof( engine_PythonDrawableObject, width ), 0, "width of rect" },
    { "height", T_INT, offsetof( engine_PythonDrawableObject, height ), 0, "height of rect" },
    { NULL }  /* Sentinel */
};

PyMethodDef CDrawableBuilder::PythonDrawable_methods[] = {
    { "makeRed", (PyCFunction)PythonDrawable_makeRed, METH_VARARGS, "Make the object red" },
    { NULL }  /* Sentinel */
};

PyGetSetDef CDrawableBuilder::PythonDrawable_getseters[] = {
    { "color",
    (getter)PythonDrawable_get_color, (setter)PythonDrawable_set_width,
    "color of the figure",
    NULL },
    { "width",
    (getter)PythonDrawable_get_width, (setter)PythonDrawable_set_width,
    "width of the figure",
    NULL },
    { "height",
    (getter)PythonDrawable_get_height, (setter)PythonDrawable_set_height,
    "height of the figure",
    NULL },
    { "xPos",
    (getter)PythonDrawable_get_xPos, (setter)PythonDrawable_set_xPos,
    "xPos of the figure",
    NULL },
    { "yPos",
    (getter)PythonDrawable_get_yPos, (setter)PythonDrawable_set_yPos,
    "yPos of the figure",
    NULL },
    { NULL }  /* Sentinel */
};

PyTypeObject CDrawableBuilder::engine_PythonDrawableType = {
    PyVarObject_HEAD_INIT( NULL, 0 )
    "Engine.CDrawable",             /* tp_name */
    sizeof( engine_PythonDrawableObject ), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PythonDrawable_dealloc,      /* tp_dealloc */
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
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                            /* tp_richcompare */
    0,                            /* tp_weaklistoffset */
    0,                            /* tp_iter */
    0,                            /* tp_iternext */
    PythonDrawable_methods,             /* tp_methods */
    PythonDrawable_members,             /* tp_members */
    PythonDrawable_getseters,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PythonDrawable_init,  /* tp_init */
    0,                         /*tp_alloc*/
    PythonDrawable_new              /*tp_new*/
};

PyObject *CDrawableBuilder::GetRawpObjectRef() const
{
    return pObject.get();
}

std::shared_ptr<PyObject> CDrawableBuilder::GetpObject() const
{
    return pObject;
}

unsigned long CDrawableBuilder::PythonDrawable_get_color( engine_PythonDrawableObject *self, void *closure )
{
    return self->color;
}

int CDrawableBuilder::PythonDrawable_get_width( engine_PythonDrawableObject *self, void *closure )
{
    return self->width;
}

int CDrawableBuilder::PythonDrawable_get_height( engine_PythonDrawableObject *self, void *closure )
{
    return self->height;
}

int CDrawableBuilder::PythonDrawable_get_xPos( engine_PythonDrawableObject *self, void *closure )
{
    return self->xPos;
}

int CDrawableBuilder::PythonDrawable_get_yPos( engine_PythonDrawableObject *self, void *closure )
{
    return self->yPos;
}

/*setters for the object*/
int CDrawableBuilder::PythonDrawable_set_color( engine_PythonDrawableObject *self, unsigned long value, void *closure )
{
    self->color = value;
    return 0;
}

int CDrawableBuilder::PythonDrawable_set_width( engine_PythonDrawableObject *self, int value, void *closure )
{
    self->width = value;
    return 0;
}

int CDrawableBuilder::PythonDrawable_set_height( engine_PythonDrawableObject *self, int value, void *closure )
{
    self->height = value;
    return 0;
}

int CDrawableBuilder::PythonDrawable_set_xPos( engine_PythonDrawableObject *self, int value, void *closure )
{
    self->xPos = value;
    return 0;
}

int CDrawableBuilder::PythonDrawable_set_yPos( engine_PythonDrawableObject *self, int value, void *closure )
{
    self->yPos = value;
    return 0;
}

PyModuleDef CDrawableBuilder::PythonDrawableModule = {
    PyModuleDef_HEAD_INIT,
    "noddy2",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyObject* CDrawableBuilder::PyInit_PythonDrawable( void )
{
    PyObject* m;

    if( PyType_Ready( &engine_PythonDrawableType ) < 0 )
        return NULL;

    m = PyModule_Create( &PythonDrawableModule );
    if( m == NULL )
        return NULL;

    Py_INCREF( &engine_PythonDrawableType );
    PyModule_AddObject( m, "Drawable", (PyObject *)&engine_PythonDrawableType );
    return m;
}