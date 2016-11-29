#include <stdafx.h>
#include "Viewer.h"

CViewer::CViewer( CStage _stage )
    : stage( _stage ), canvas(), viewerWindow( stage, canvas )
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