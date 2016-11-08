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


std::vector<int> CScriptEngine::RunScripts( const IdType objectId, const std::vector<CScript>& scripts, EventType eventType )
{
    if( !isPythonRunning )
	{
		Py_Initialize(); //starting up Python if first run
		holder = std::make_shared<ScriptHolder>(ScriptHolder());
		isPythonRunning = true;
	}
    else if( objectId == CObjectIdGenerator::GetEmptyId() )
	{
		Py_Finalize(); //finlazing Python before turning the programm off
		holder->decAllRefsAndClearObjects();
		return std::vector<int>();
	}
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById(objectId);
	
	// Here was process of creation of PyObject,
	// but for now we decided to use Dirs due to simplicity


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
		CScriptSolver solver( workingObject, scriptNameWithoutExtention, eventType, holder );

        std::shared_ptr<IDrawable> changedObject = solver.Run();   //Returns shared_ptr to changed object, but values already set in the scene
		assert( changedObject == workingObject );
	}
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
