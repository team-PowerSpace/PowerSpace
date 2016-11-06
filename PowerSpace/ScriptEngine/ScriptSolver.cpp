#include <stdafx.h>
#include "ScriptSolver.h"
#include <iostream>
#include <exception>

CScriptSolver::CScriptSolver( std::shared_ptr<IDrawable> obj, std::wstring scriptName_,
	EventType eventType_, std::shared_ptr<ScriptHolder> holder_ ) : scriptName( scriptName_ ), eventType( eventType_ ), object( obj ), holder( holder_ )
{
	pObject = std::make_shared<CDrawableBuilder>(obj);
}

std::shared_ptr<IDrawable> CScriptSolver::Run()
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	if( !holder->isScriptIn( scriptName ) ) {
		pName = PyUnicode_FromUnicode( scriptName.c_str(), scriptName.size() );
		pModule = PyImport_Import( pName );
		holder->addScript( scriptName, pModule );
		//Because the pyimport creates 2 ref for unrealiesd reason
		Py_XDECREF( pModule );
		Py_XDECREF( pName );
	}
	pModule = holder->getScript(scriptName);

	if( pModule != nullptr ) {
		pFunc = GetPyFunction( pModule );
		Py_XDECREF(pModule);
		if( pFunc && PyCallable_Check( pFunc ) ) {
			pArgs = PyTuple_New( 1 );
			PyTuple_SetItem( pArgs, 0, pObject->GetRawpObjectRef() );
			pValue = PyObject_CallObject( pFunc, pArgs );
			Py_XDECREF( pValue );
			Py_CLEAR( pFunc );
		}
	}
	UpdateObject();
	return object;
}

PyObject *SafeGetFunc( PyObject *pModule, const char *funcName )
{
	if ( PyObject_HasAttrString( pModule, funcName ) )
	{
		return PyObject_GetAttrString( pModule, funcName );
	}
	else {
		return NULL;
	}
}

PyObject *CScriptSolver::GetPyFunction( PyObject *pModule ) const
{
	//Strings will be added to enum (need to discuss location)
	PyObject *pFunc = NULL;
	switch ( eventType )
	{
		case EventType::EventClick:
		{
			pFunc = SafeGetFunc( pModule, "OnClick" );
			break;
		}
		case EventType::EventTick:
		{
			pFunc = SafeGetFunc( pModule, "OnTimer" );
			break;
		}
		default:
			assert( false );
	}
	return pFunc;
}

void CScriptSolver::UpdateObject()
{
	PyObject* pythonObject = pObject->GetRawpObjectRef();

	unsigned long color = pObject->PythonDrawable_get_color(
		reinterpret_cast<CDrawableBuilder::engine_PythonDrawableObject *>(pythonObject), 0 );
	int xPos = pObject->PythonDrawable_get_xPos(
		reinterpret_cast<CDrawableBuilder::engine_PythonDrawableObject *>(pythonObject), 0 );
	int yPos = pObject->PythonDrawable_get_yPos(
		reinterpret_cast<CDrawableBuilder::engine_PythonDrawableObject *>(pythonObject), 0 );
	int width = pObject->PythonDrawable_get_width(
		reinterpret_cast<CDrawableBuilder::engine_PythonDrawableObject *>(pythonObject), 0 );
	int height = pObject->PythonDrawable_get_height(
		reinterpret_cast<CDrawableBuilder::engine_PythonDrawableObject *>(pythonObject), 0 );

	object->SetColor(color);
	RECT rect;
	rect.top = yPos;
	rect.left = xPos;
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;
	object->SetContainingBox(rect);
}