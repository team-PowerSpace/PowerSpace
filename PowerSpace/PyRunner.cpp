#include <stdafx.h>
#include <Python.h>
#include <map>
#include "PyRunner.h"

void PyRunner::run( IdType object, EventType type, std::vector<IdType> scripts )
{
	for( IdType script : scripts ) {
		std::string buffer = std::string(script.begin(), script.end() );
		buffer.append( "." );
		IdType event = Events.at( type );
		buffer.append( std::string( event.begin(), event.end() ) );
		buffer.append( "(" );
		buffer.append( std::string( object.begin(), object.end() ) );
		buffer.append( ")" );		
		int result = PyRun_SimpleString( buffer.c_str() ); 
		if( result == -1 ) {
			throw "Error: failed to run script";
		}
	}	 
}

void PyRunner::update( std::shared_ptr<CDrawableBuilder> pObject, std::shared_ptr<IDrawable> cObject )
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

	cObject->SetColor( color );
	RECT rect;
	rect.top = yPos;
	rect.left = xPos;
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;
	cObject->SetContainingBox( rect );
}