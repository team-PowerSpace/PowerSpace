#pragma once
#include <Python.h>
#include <Windows.h>
#include "structmember.h"
#include "../Common/Drawable.h"
#include <memory>



class CDrawableBuilder
{
public:
	CDrawableBuilder( std::shared_ptr<IDrawable> object );

	typedef struct
	{
		PyObject_HEAD
			unsigned long int color;
		int xPos, yPos, width, height;
	} engine_PythonDrawableObject;

	static void PythonDrawable_makeRed( engine_PythonDrawableObject* self );

	static void PythonDrawable_dealloc( engine_PythonDrawableObject* self );

	static int PythonDrawable_init( engine_PythonDrawableObject *self, PyObject *args,
		PyObject *kwds );

	static PyObject *PythonDrawable_new(PyTypeObject *type, PyObject *args,
		PyObject *kwds);
	
	static PyMemberDef PythonDrawable_members[];

	static PyMethodDef PythonDrawable_methods[];

	static PyGetSetDef PythonDrawable_getseters[];

	static PyTypeObject engine_PythonDrawableType;

	/*getters for the object*/
	
	PyObject *GetRawpObjectRef() const;

	std::shared_ptr<PyObject> GetpObject() const;

	static unsigned long PythonDrawable_get_color(engine_PythonDrawableObject *self, void *closure);

	static int PythonDrawable_get_width(engine_PythonDrawableObject *self, void *closure);

	static int PythonDrawable_get_height(engine_PythonDrawableObject *self, void *closure);

	static int PythonDrawable_get_xPos(engine_PythonDrawableObject *self, void *closure);

	static int PythonDrawable_get_yPos(engine_PythonDrawableObject *self, void *closure);

	/*setters for the object*/
	static int PythonDrawable_set_color(engine_PythonDrawableObject *self, unsigned long value, void *closure);

	static int PythonDrawable_set_width(engine_PythonDrawableObject *self, int value, void *closure);

	static int PythonDrawable_set_height(engine_PythonDrawableObject *self, int value, void *closure);

	static int PythonDrawable_set_xPos(engine_PythonDrawableObject *self, int value, void *closure);

	static int PythonDrawable_set_yPos(engine_PythonDrawableObject *self, int value, void *closure);
	
private:
	std::shared_ptr<PyObject> pObject;
};

