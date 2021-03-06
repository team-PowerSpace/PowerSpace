#pragma once
#include <Stage.h>
#include <ViewerWindow.h>

class CViewer
{
public:
    // construct viewer object on the base of stage;
    // viewer must have its own copy of the stage since it can modify objects while running scripts.
    CViewer( CStage _stage );
    ~CViewer();

    // create win32 window
    bool Create();

    void SetStage( CStage _stage );

    CStage& GetStage();

    const CViewerWindow& GetViewerWindow() const;

private:
    // copy of stage
    CStage stage;

    // canvas upon which the objects are placed
    CCanvas canvas;

    // win32 window of the viewer
    CViewerWindow viewerWindow;
};
