#include <stdafx.h>
#include "Viewer.h"

CViewer::CViewer( CStage _stage )
    : stage( _stage ), canvas(), viewerWindow( stage, canvas )
{}

CViewer::~CViewer()
{
    try {
        Py_Finalize(); //shutting down Python here
    } catch( ... ) {
        ::MessageBox(0, L"Python finalization crashed", L"Crash", MB_OK);
    }
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