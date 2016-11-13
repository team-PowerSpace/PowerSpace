#include <stdafx.h>
#include <CViewPortWrap.h>
#pragma warning( disable : 4100 )

CViewPortWrap::CViewPortWrap( const CViewport& viewport )
{
	if( PyType_Ready( &engine_PythonViewportType ) < 0 ) {
		pObject = nullptr;
	} else {
		pObject = std::shared_ptr<PyObject>(
			reinterpret_cast<PyObject *>(PyObject_NEW( engine_PythonViewportObject, &engine_PythonViewportType)),
			[=] ( PyObject *pObject )
		{
			PyObject_FREE(pObject);
		});

		PythonViewport_set_scale( 
			reinterpret_cast<engine_PythonViewportObject *>( pObject.get() ), viewport.GetScale(), 0 );
        PythonViewport_set_angle (
			reinterpret_cast<engine_PythonViewportObject *>( pObject.get() ), viewport.GetAngle(), 0 );
        auto zeroLocation = viewport.GetZeroLocation();
		PythonViewport_set_xPos( 
			reinterpret_cast<engine_PythonViewportObject *>( pObject.get() ), zeroLocation.x, 0 );
		PythonViewport_set_yPos( 
			reinterpret_cast<engine_PythonViewportObject *>( pObject.get() ), zeroLocation.y, 0 );
	}
}


void CViewPortWrap::PythonViewport_dealloc( engine_PythonViewportObject* self )
{
	Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>(self) );
}

int CViewPortWrap::PythonViewport_init( engine_PythonViewportObject *self, PyObject *args,
	PyObject *kwds )
{
	static char *kwlist[] = { "scale", "angle", "xPos", "yPos", NULL };
	if( !PyArg_ParseTupleAndKeywords( args, kwds, "|ffii", kwlist,
		&self->scale, &self->angle, &self->xPos, &self->yPos ) ) {
		return -1;
	}
	return 0;
}

PyObject *CViewPortWrap::PythonViewport_new(PyTypeObject *type, PyObject *args,
	PyObject *kwds)
{
	engine_PythonViewportObject *self;
	self = (engine_PythonViewportObject *)type->tp_alloc(type, 0);
	if (self) {
		self->scale = 1.0;
		self->angle = 0.0;
		self->xPos = 0;
		self->yPos = 0;
	}
	return reinterpret_cast<PyObject *>(self);
}

PyMemberDef CViewPortWrap::PythonViewport_members[] = {
	{ "scale", T_FLOAT, offsetof( engine_PythonViewportObject, scale ), 0, "scale" },
	{ "angle", T_FLOAT, offsetof( engine_PythonViewportObject, angle), 0, "angle" },
	{ "xPos", T_INT, offsetof( engine_PythonViewportObject, xPos ), 0, "xPos of left top corner" },
	{ "yPos", T_INT, offsetof( engine_PythonViewportObject, yPos ), 0, "yPos of left top corner" },
	{ NULL }  /* Sentinel */
};

PyMethodDef CViewPortWrap::PythonViewport_methods[] = {
	{ NULL }  /* Sentinel */
};

PyGetSetDef CViewPortWrap::PythonViewport_getseters[] = {
	{ "scale",
	(getter)PythonViewport_get_scale, (setter)PythonViewport_set_scale,
	"scale of viewport",
	NULL },
	{ "angle",
	(getter)PythonViewport_get_angle, (setter)PythonViewport_set_angle,
	"Angle of viewport rotation",
	NULL },
	{ "xPos",
	(getter)PythonViewport_get_xPos, (setter)PythonViewport_set_xPos,
	"x_position of zero location",
	NULL },
	{ "yPos",
	(getter)PythonViewport_get_yPos, (setter)PythonViewport_set_yPos,
	"y_position of zero location",
	NULL },
	{ NULL }  /* Sentinel */
};

PyTypeObject CViewPortWrap::engine_PythonViewportType = {
	PyVarObject_HEAD_INIT( NULL, 0 )
	"Engine.Viewport",             /* tp_name */
	sizeof( engine_PythonViewportObject ), /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)PythonViewport_dealloc,      /* tp_dealloc */
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
	PythonViewport_methods,             /* tp_methods */
	PythonViewport_members,             /* tp_members */
	PythonViewport_getseters,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)PythonViewport_init,  /* tp_init */
	0,                         /*tp_alloc*/
	PythonViewport_new              /*tp_new*/
};

PyObject *CViewPortWrap::GetRawpObjectRef() const
{
	return pObject.get();
}

std::shared_ptr<PyObject> CViewPortWrap::GetpObject() const
{
	return pObject;
}

float CViewPortWrap::PythonViewport_get_scale(engine_PythonViewportObject *self, void *closure)
{
	return self->scale;
}

float CViewPortWrap::PythonViewport_get_angle(engine_PythonViewportObject *self, void *closure)
{
	return self->angle;
}

int CViewPortWrap::PythonViewport_get_xPos(engine_PythonViewportObject *self, void *closure)
{
	return self->xPos;
}

int CViewPortWrap::PythonViewport_get_yPos(engine_PythonViewportObject *self, void *closure)
{
	return self->yPos;
}

/*setters for the object*/
int CViewPortWrap::PythonViewport_set_scale(engine_PythonViewportObject *self, float value, void *closure)
{
	self->scale = value;
	return 0;
}

int CViewPortWrap::PythonViewport_set_angle(engine_PythonViewportObject *self, float value, void *closure)
{
	self->angle = value;
	return 0;
}

int CViewPortWrap::PythonViewport_set_xPos(engine_PythonViewportObject *self, int value, void *closure)
{
	self->xPos = value;
	return 0;
}

int CViewPortWrap::PythonViewport_set_yPos(engine_PythonViewportObject *self, int value, void *closure)
{
	self->yPos = value;
	return 0;
}