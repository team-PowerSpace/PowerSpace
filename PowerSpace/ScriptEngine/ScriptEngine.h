#pragma once
#include "../Common/Stage.h"

// this class processes python scripts and changes the stage
class CScriptEngine
{
public:
    explicit CScriptEngine( CStage& _stage );

    // evaluates python scripts from "scripts", modifying the stage;
    // objectId - id of the object, which caused the event
    // returns the vector of objects that were modified during the evaluation of the scripts
    std::vector<int> RunScripts( int objectId, const std::vector<CScript>& scripts );
private:
	void setNewValues(CDrawableBuilder::CDrawableStruct& newData, std::shared_ptr<IDrawable> workingObject);
    CStage& stage;
};
