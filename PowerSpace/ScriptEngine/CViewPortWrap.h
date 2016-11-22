#pragma once
#include <Python.h>
#include <Windows.h>
#include "structmember.h"
#include <Viewport.h>
#include <memory>



class CViewPortWrap
{
public:
    CViewPortWrap( const CViewport& viewport );

    typedef struct
    {
        PyObject_HEAD
            double scale;
        double angle;
        int xPos, yPos;
    } engine_PythonViewportObject;

    static void PythonViewport_dealloc( engine_PythonViewportObject* self );

    static int PythonViewport_init( engine_PythonViewportObject *self, PyObject *args,
        PyObject *kwds );

    static PyObject *PythonViewport_new( PyTypeObject *type, PyObject *args,
        PyObject *kwds );

    static PyMemberDef PythonViewport_members[];

    static PyMethodDef PythonViewport_methods[];

    static PyGetSetDef PythonViewport_getseters[];

    static PyTypeObject engine_PythonViewportType;

    /*getters for the object*/

    PyObject *GetRawpObjectRef() const;

    std::shared_ptr<PyObject> GetpObject() const;

    static double PythonViewport_get_scale( engine_PythonViewportObject *self, void *closure );

    static double PythonViewport_get_angle( engine_PythonViewportObject *self, void *closure );

    static int PythonViewport_get_xPos( engine_PythonViewportObject *self, void *closure );

    static int PythonViewport_get_yPos( engine_PythonViewportObject *self, void *closure );

    /*setters for the object*/
    static int PythonViewport_set_scale( engine_PythonViewportObject *self, double value, void *closure );

    static int PythonViewport_set_angle( engine_PythonViewportObject *self, double value, void *closure );

    static int PythonViewport_set_xPos( engine_PythonViewportObject *self, int value, void *closure );

    static int PythonViewport_set_yPos( engine_PythonViewportObject *self, int value, void *closure );

private:
    std::shared_ptr<PyObject> pObject;
};

