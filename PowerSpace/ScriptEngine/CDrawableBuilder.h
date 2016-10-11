#pragma once
#include <Python.h>
#include <Windows.h>
#include "structmember.h"
#include "../Common/Drawable.h"
#include <memory>

class CDrawableBuilder {
public:
	CDrawableBuilder(std::shared_ptr<IDrawable> object);	
	std::shared_ptr<PyObject> GetpObject();

	typedef struct {
		PyObject_HEAD
			unsigned long int color;
		int xPos, yPos, width, height;
	} CDrawableStruct;

	static void CDrawableRedable(CDrawableStruct* self); 

	static void CDrawableDealloc(CDrawableStruct* self);	

	static int CDrawableInit(CDrawableStruct *self, PyObject *args, 
		PyObject *kwds);

	static PyMemberDef CDrawableMembers[]; 	
	
	static PyMethodDef CDrawableMethods[];

	static PyTypeObject CDrawableType; 
private:
	std::shared_ptr<PyObject> pObject;
};

