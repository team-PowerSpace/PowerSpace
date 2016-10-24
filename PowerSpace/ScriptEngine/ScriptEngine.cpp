﻿#include <stdafx.h>
#include "ScriptEngine.h"
#include "ScriptSolver.h"
#include "PyObjectBuilder.h"
#include "CDrawableBuilder.h"
#include "CScriptHolder.h"
#include <codecvt>
#include <Exception>
#include <iostream>
#include <fstream>

CScriptEngine::CScriptEngine( CStage& _stage )
    : stage( _stage )
{}


std::vector<int> CScriptEngine::RunScripts(int objectId, const std::vector<CScript>& scripts )
{
	if (!isPythonRunning)
	{
		Py_Initialize(); //starting up Python if first run
		ScriptHolder sHolder();
		holder = std::shared_ptr<ScriptHolder> (sHolder);
		isPythonRunning = true;
	}
	else if (objectId == -1) 
	{
		Py_Finalize(); //finlazing Python before turning the programm off
		return std::vector<int>();
	}
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById(objectId);
	
	// Here was process of creation of PyObject,
	// but for now we decided to use Dirs due to simplicity


	std::string emptyString = ""; //see comment to line 55


	for (auto i = scripts.begin(); i != scripts.end(); i++)
	{
		TPath wstrPath((*i).GetPath());
		std::ifstream stream(wstrPath.data(), std::ifstream::in); //The best way to chek path validity is trying to open it
		if (!stream.good())
		{
			stream.close();
			std::cout << "The file doesn't exist" << std::endl;
			assert(false);
		}
		stream.close();

		//Deleteing the path and saving only script's filename (with extention)
		std::wstring scriptName = wstrPath.substr(wstrPath.find_last_of(L"\\/") + 1);

		//Deleting the extention of script
		std::wstring scriptNameWithoutExtention = scriptName.substr(0, scriptName.find(L"."));

		//Empty string left for ability to call different functions located in single script
		//For now we send nothing instead of name of function, to find out why -- read comment
		//is ScriptSolver, line 41
        CScriptSolver solver( workingObject, scriptNameWithoutExtention, /*std::string("OnClick")*/std::string(""), holder);
        
        std::shared_ptr<IDrawable> changedObject = solver.Run();   //Returns shared_ptr to changed object, but values already set in the scene
		assert(changedObject == workingObject);
	}
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
