#include <stdafx.h>
#include "Viewer.h"

CViewer::CViewer( CStage _stage, CViewport _viewport )
    : stage( _stage ), canvas(), viewport( _viewport ), viewerWindow(viewport, canvas)
{
}

CViewer::~CViewer()
{}

bool CViewer::Create()
{
    bool isCreated = viewerWindow.Create();
    viewerWindow.Show();
	return isCreated;
}

void CViewer::SetStage( CStage _stage )
{
	stage = _stage;
}

CStage& CViewer::GetStage()
{
	return stage;
}
