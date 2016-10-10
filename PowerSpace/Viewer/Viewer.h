#pragma once
#include "Stage.h"
#include "ViewerWindow.h"

class CViewer
{
public:
    // construct viewer object on the base of stage;
    // viewer must have its own copy of the stage since it can modify objects while running scripts.
    CViewer( CStage _stage, CViewport _viewport );
	~CViewer();

	// create win32 window
	bool Create();

	void SetStage( CStage _stage );

	CStage& GetStage();

private:
	// copy of stage
    CStage stage;

	// canvas upon which the objects are placed
	CCanvas canvas;

	// viewable part of stage, copied from editor, independent
	CViewport viewport;

	// win32 window of the viewer
	CViewerWindow viewerWindow;

	void tickHandler();

	void clickHandler();
};
