#include <stdafx.h>
#include "Editor.h"
#include "EditorWindow.h"
#include "Stage.h"
#include "StageObjects.h"


#pragma warning(push)
#pragma warning(disable:4100)
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if( !CEditor::RegisterClass() ) {
		return 1;
	}
	CEditor window;
	if( !window.Create() ) {
		return 3;
	}

	// code for testing. May be deleted
	std::shared_ptr<CStage> stage = std::make_shared<CStage>();
	TBox box;
	box.left = 10;
	box.top = 10;
	box.right = 100;
	box.bottom = 200;
	CRectangleObject rect( RGB( 11, 22, 33 ), box );
	stage->GetObjects().insert( std::pair<int, std::shared_ptr<IDrawable>>( 0, std::dynamic_pointer_cast<IDrawable,
		CRectangleObject>(std::make_shared<CRectangleObject>( RGB( 11, 22, 33 ), box )) ) );
	 window.SetStage( stage );

	window.Show(nCmdShow);

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