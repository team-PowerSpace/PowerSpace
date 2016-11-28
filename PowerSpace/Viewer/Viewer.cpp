#include <stdafx.h>
#include "Viewer.h"

CViewer::CViewer( CStage _stage, CViewport _viewport )
    : stage( _stage ), canvas(), viewport( _viewport ), \
    viewerWindow( stage, viewport, canvas )
{}

CViewer::~CViewer()
{
    Py_Finalize(); //shutting down Python here
}

bool CViewer::Create()
{
    bool isCreated = viewerWindow.Create();
    if( isCreated ) {
        viewerWindow.Show();
    }
    Py_Initialize(); //starting up Python right here
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

const CViewerWindow& CViewer::GetViewerWindow() const
{
    return viewerWindow;
}