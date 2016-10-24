#include <stdafx.h>
#include "ScriptEngine.h"
#include "ScriptSolver.h"
#include "PyObjectBuilder.h"
#include "CDrawableBuilder.h"
#include <codecvt>
#include <Exception>
#include <iostream>
#include <fstream>

CScriptEngine::CScriptEngine( CStage& _stage )
	: stage( _stage ), isPythonRunning( false )
{}


std::vector<int> CScriptEngine::RunScripts( const int objectId, const std::vector<CScript>& scripts )
{
	if( !isPythonRunning ) {
		Py_Initialize(); //starting up Python if first run
	} else if( objectId == -1 ) {
		Py_Finalize(); //finlazing Python before turning the programm off
		return std::vector<int>();
	}
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById( objectId );

	// Here was process of creation of PyObject,
	// but for now we decided to use Dirs due to simplicity

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter; //to convert from TPath (std::wstring) to std::string

	for( auto currentScript = scripts.begin(); currentScript != scripts.end(); currentScript++ ) {
		TPath wstrPath( currentScript->GetPath() );
		std::ifstream stream( wstrPath.data(), std::ifstream::in ); //The best way to check path validity is trying to open it
		if( !stream.good() ) {
			stream.close();
			std::cout << "The file doesn't exist" << std::endl;
			assert( false );
		}
		stream.close();

		//Deleteing the path and saving only script's filename (with extention)
		std::wstring scriptName = wstrPath.substr( wstrPath.find_last_of( L"\\/" ) + 1 );

		//Deleting the extention of script
		std::wstring scriptNameWithoutExtention = scriptName.substr( 0, scriptName.find( L"." ) );


		//Empty string left for ability to call different functions located in single script
		std::string emptyString = "";
		CScriptSolver solver( workingObject, scriptNameWithoutExtention, emptyString );

		std::shared_ptr<IDrawable> changedObject = solver.RunWithDict();   //Returns shared_ptr to changed object, but values already set in the scene
		assert( changedObject == workingObject );
	}
	return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
