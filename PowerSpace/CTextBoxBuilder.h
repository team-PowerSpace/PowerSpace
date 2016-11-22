#pragma once
#include "CDrawableBuilder.h"

class CTextBoxBuilder {
public:
	CTextBoxBuilder( std::shared_ptr<IDrawable> object );

	typedef struct {
		PyObject_HEAD
			std::string text;
		unsigned int fontSize;
		unsigned long int fontColor;
	} engine_PythonTextBoxObject;

	static void PythonTextBox_dealloc( engine_PythonTextBoxObject* self );

	static int PythonTextBox_init( engine_PythonTextBoxObject *self, PyObject *args,
		PyObject *kwds );

	static PyObject *PythonTextBox_new( PyTypeObject *type, PyObject *args,
		PyObject *kwds );

	static PyMemberDef PythonTextBox_members[];

	static PyMethodDef PythonTextBox_methods[];

	static PyGetSetDef PythonTextBox_getseters[];

	static PyTypeObject engine_PythonTextBoxType;

	/*getters for the object*/

	PyObject *GetRawpObjectRef() const;

	std::shared_ptr<PyObject> GetpObject() const;

	static std::string PythonTextBox_get_text( engine_PythonTextBoxObject *self, void *closure );

	static unsigned int PythonTextBox_get_fontSize( engine_PythonTextBoxObject *self, void *closure );

	static unsigned long int PythonTextBox_get_fontColor( engine_PythonTextBoxObject *self, void *closure );

	/*setters for the object*/

	static int PythonTextBox_set_text( engine_PythonTextBoxObject *self, std::string value, void *closure );

	static int PythonTextBox_set_fontSize( engine_PythonTextBoxObject *self, unsigned int, void *closure );

	static int PythonTextBox_set_fontColor( engine_PythonTextBoxObject *self, unsigned long int value, void *closure );

private:
	std::shared_ptr<PyObject> pObject;
};