#pragma once
#include "../Common/Stage.h"

class CViewer
{
public:
    // construct viewer object on the base of stage;
    // viewer must have its own copy of the stage since it can modify objects while running scripts.
    explicit CViewer( CStage _stage );
private:
    CStage stage;
};
