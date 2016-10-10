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
	return viewerWindow.Create();
}

void CViewer::SetStage( CStage _stage )
{
	stage = _stage;
}

CStage& CViewer::GetStage()
{
	return stage;
}
