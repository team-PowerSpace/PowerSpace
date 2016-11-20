#include <stdafx.h>
#include "Editor.h"
#include "EditorWindow.h"
#include "Stage.h"
#include "StageObjects.h"
#include "ViewerWindow.h"
#include "ScriptEditor.h"
#include <CScriptHolder.h>

#pragma warning(push)
#pragma warning(disable:4100)
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    if( !CEditor::RegisterClass() || !CViewerWindow::RegisterClass() || !CScriptEditor::RegisterClass() ) {
        return 1;
    }
    CEditor window;
    if( !window.Create() ) {
        return 3;
    }

    std::shared_ptr<CStage> stage = std::make_shared<CStage>();
    std::shared_ptr<CScriptHolder> holder = std::make_shared<CScriptHolder>();
    window.SetStage( stage );
    window.SetHolder( holder );
    window.Show( nCmdShow );

    MSG message;
    BOOL getMessageResult = 0;
    while( (getMessageResult = ::GetMessage( &message, 0, 0, 0 )) != 0 ) {
        if( getMessageResult == -1 ) {
            return 4;
        }
        ::TranslateMessage( &message );
        ::DispatchMessage( &message );
    }

    return 0;
}
#pragma warning(pop)