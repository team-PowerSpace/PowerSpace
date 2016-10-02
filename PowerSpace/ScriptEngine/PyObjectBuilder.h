#pragma once
#include <Python.h>
#include "Common\Drawable.h"
#include <memory>

class CPyObjectBuilder {
public:
	CPyObjectBuilder(std::shared_ptr<IDrawable> object);
	PyObject* GetpObject();
private:
	PyObject* pObject;
};