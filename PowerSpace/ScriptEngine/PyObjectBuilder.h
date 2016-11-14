#pragma once
#include <Python.h>
#include <Drawable.h>
#include <memory>

class CPyObjectBuilder
{
public:
	CPyObjectBuilder( std::shared_ptr<IDrawable> object );
	std::shared_ptr<PyObject> GetpObject();
private:
	std::shared_ptr<PyObject> pObject;
};