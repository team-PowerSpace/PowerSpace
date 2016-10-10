#include "../stdafx.h"
#include "ScriptEngine.h"
#include "ScriptSolver.h"
#include "PyObjectBuilder.h"
#include "CDrawableBuilder.h"
#include <codecvt>
#include <Exception>
#include <iostream>
#include <fstream>

CScriptEngine::CScriptEngine( CStage& _stage )
    : stage( _stage )
{}


std::vector<int> CScriptEngine::RunScripts(int objectId, const std::vector<CScript>& scripts )
{
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById(objectId);
	
	// Here was process of creation of PyObject,
	// but for now we decided to use Dirs due to simplicity

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter; //to convert from TPath (std::wstring) to std::string

	std::string emptyString = ""; //see comment to line 41

	for (auto i = scripts.begin(); i != scripts.end(); i++)
	{
		TPath wstrPath((*i).GetPath());
		std::string strPath = converter.to_bytes(wstrPath);
		std::ifstream stream(strPath.data(), std::ifstream::in); //The best way to chek path validity is trying to open it
		if (!stream)
		{
			stream.close();
			std::cout << "The file doesn't exist" << std::endl;
			assert(false);
		}
		stream.close();

		CScriptSolver solver(workingObject, strPath, emptyString); //Empty string left for ability to call different functions located in single script
		std::shared_ptr<IDrawable> changedObject = solver.Run();   //Returns shared_ptr to changed object, but values already set in the scene
		assert(changedObject == workingObject);
	}
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
