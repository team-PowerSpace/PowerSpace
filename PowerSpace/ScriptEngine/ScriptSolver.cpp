#include <stdafx.h>
#include "ScriptSolver.h"
#include <iostream>
#include <exception>

CScriptSolver::CScriptSolver( std::shared_ptr<IDrawable> obj, std::wstring scriptName_,
	std::string func_ ) : scriptName( scriptName_ ), func( func_ ), object( obj )
{
	//Will be deleted after the testing of PyObject based solution
	std::pair<std::string, long> xPos( "x", obj->GetContainingBox().left );
	std::pair<std::string, long> yPos( "y", obj->GetContainingBox().top );
	std::pair<std::string, long> width( "width", obj->GetContainingBox().right - obj->GetContainingBox().left );
	std::pair<std::string, long> heigth( "height", obj->GetContainingBox().bottom - obj->GetContainingBox().top );
	std::pair<std::string, long> color( "color", obj->GetColor() );
	members.insert( xPos );
	members.insert( yPos );
	members.insert( width );
	members.insert( heigth );
	members.insert( color );

	//Creating and initialising PyObject from object

}

std::shared_ptr<IDrawable> CScriptSolver::RunWithDict()
{
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;

	pName = PyUnicode_FromUnicode( scriptName.c_str(), scriptName.size() );
	pModule = PyImport_Import( pName );
	Py_DECREF( pName );

	if( pModule != nullptr ) {
		pFunc = PyObject_GetAttrString( pModule, func.c_str() );
		if( pFunc && PyCallable_Check( pFunc ) ) {
			pArgs = PyTuple_New( 1 );
			pDict = PyDict_New();
			FillDict( pDict );
			PyTuple_SetItem( pArgs, 0, pDict );
			pValue = PyObject_CallObject( pFunc, pArgs );
			UpdateDict( pDict );
			Py_XDECREF( pValue );
		}

		Py_XDECREF( pFunc );
		Py_DECREF( pModule );
	}
	object->SetColor( members.find( "color" )->second );
	RECT rect;
	rect.top = members.find( "y" )->second;
	rect.left = members.find( "x" )->second;
	rect.bottom = rect.top + members.find( "height" )->second;
	rect.right = rect.left + members.find( "width" )->second;
	object->SetContainingBox( rect );
	return object;
}

std::shared_ptr<IDrawable> CScriptSolver::Run()
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	//temprorary hard code will be replaced by using script manager {
	pName = PyUnicode_FromUnicode( scriptName.c_str(), scriptName.size() );
	pModule = PyImport_Import( pName );
	Py_DECREF( pName );
	//}

	if( pModule != nullptr ) {
		pFunc = GetPyFunction( pModule );
		if( pFunc && PyCallable_Check( pFunc ) ) {
			pArgs = PyTuple_New( 1 );
			PyTuple_SetItem( pArgs, 0, pObject->GetpObject().get() );
			pValue = PyObject_CallObject( pFunc, pArgs );
			Py_XDECREF( pValue );
		}

		Py_XDECREF( pFunc );
		Py_DECREF( pModule );
	}
	UpdateObject();
	return object;
}

void CScriptSolver::FillDict( PyObject* dict )
{
	for( auto member : members ) {
		PyDict_SetItem( dict, Py_BuildValue( "s", member.first.c_str() ),
			Py_BuildValue( "l", member.second ) );
	}
}

void CScriptSolver::UpdateDict( PyObject* dict )
{
	PyObject* keys = PyDict_Keys( dict );
	members.clear();
	for( int i = 0; i < PyList_Size( keys ); i++ ) {
		PyObject* key = PyList_GetItem( keys, i );
		PyObject* val = PyDict_GetItem( dict, key );
		std::pair<std::string, long> pair( std::string( PyUnicode_AsUTF8( key ) ),
			PyLong_AsLong( val ) );
		members.insert( pair );
	}
}

PyObject *CScriptSolver::GetPyFunction( PyObject *pModule ) const
{
	//Strings will be added to enum (need to discuss location)
	PyObject *pFunc;
	if( func == "" ) {
		pFunc = PyObject_GetAttrString( pModule, "OnClick" );
	} else {
		pFunc = PyObject_GetAttrString( pModule, func.c_str() );
	}
	if( pFunc == NULL ) {
		pFunc = PyObject_GetAttrString( pModule, "OnTimer" );
	}
	return pFunc;
}

void CScriptSolver::UpdateObject()
{
	//realisation of update
}