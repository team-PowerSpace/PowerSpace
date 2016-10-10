#include "../stdafx.h"
#include "ScriptEngine.h"
#include "ScriptSolver.h"
#include "PyObjectBuilder.h"
#include "CDrawableBuilder.h"
#include <codecvt>

CScriptEngine::CScriptEngine( CStage& _stage )
    : stage( _stage )
{}


std::vector<int> CScriptEngine::RunScripts(int objectId, const std::vector<CScript>& scripts )
{
	std::shared_ptr<IDrawable> workingObject = stage.GetObjectById(objectId);
	CPyObjectBuilder builder(workingObject);
	std::shared_ptr<PyObject> pyObject = builder.GetpObject();

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter; //to convert from TPath (std::wstring) to std::string

	std::string emptyString = "";

	for (auto i = scripts.begin(); i != scripts.end(); i++)
	{
		TPath wstrPath((*i).GetPath());
		std::string strPath = converter.to_bytes(wstrPath);
		CScriptSolver solver(workingObject, strPath, emptyString); //Empty string left for ability to call different functions located in single script
		std::shared_ptr<IDrawable> changedObject = solver.Run();   //returns shared_ptr to changed object, but values already set in the scene
	}
    return std::vector<int>(); //Not used for now, but later will allow to return list of objects changed (if needed) for 
}
