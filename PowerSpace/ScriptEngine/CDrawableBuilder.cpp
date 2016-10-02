#include "stdafx.h" 
#include "CDrawableBuilder.h"

CDrawableBuilder::CDrawableBuilder(std::shared_ptr<IDrawable> object) {	
		std::shared_ptr<CDrawable> castedObject =
			std::static_pointer_cast<CDrawable>(object);		
		if (PyType_Ready(&CDrawableType) < 0) {
			pObject = nullptr;
		}
		else {
			RECT rect = object->GetPosition();
			pObject = (PyObject*)&CDrawableType;
			PyObject* args = PyTuple_New(5);
			PyTuple_SetItem(args, 0, PyLong_FromUnsignedLong(object->GetColor()));
			PyTuple_SetItem(args, 1, PyLong_FromLong(rect.left));
			PyTuple_SetItem(args, 2, PyLong_FromLong(rect.top));
			PyTuple_SetItem(args, 3, PyLong_FromLong(rect.right - rect.left));
			PyTuple_SetItem(args, 4, PyLong_FromLong(rect.bottom - rect.top));

			PyObject* kwds = PyTuple_New(5);
			PyTuple_SetItem(args, 0, PyUnicode_FromString("color"));
			PyTuple_SetItem(args, 1, PyUnicode_FromString("xPos"));
			PyTuple_SetItem(args, 2, PyUnicode_FromString("yPos"));
			PyTuple_SetItem(args, 3, PyUnicode_FromString("width"));
			PyTuple_SetItem(args, 4, PyUnicode_FromString("heigh"));
			pObject->ob_type->tp_init(pObject, args, kwds);
		}
}

PyObject* CDrawableBuilder::GetpObject() {
	return pObject;
}
