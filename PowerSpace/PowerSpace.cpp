#include "stdafx.h"
#include "Editor\EditorWindow.h"


#pragma warning(push)
#pragma warning(disable:4100)
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if( !CEditorWindow::RegisterClass() ) {
		return 1;
	}
	CEditorWindow window;
	if( !window.Create() ) {
		return 3;
	}

	std::vector<std::shared_ptr<IDrawable>> temp;


	window.Show();

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