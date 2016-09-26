#include "stdafx.h"

int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	//if (!COverlappedWindow::RegisterClass()) {
	//	return 1;
	//}
	//COverlappedWindow window;
	//if (!window.Create()) {
	//	return 3;
	//}
	//window.Show(nCmdShow);


	//HACCEL acceleartors = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(...));
	//MSG message;
	//BOOL getMessageResult = 0;
	//while ((getMessageResult = ::GetMessage(&message, 0, 0, 0)) != 0) {
	//	if (getMessageResult == -1) {
	//		return 4;
	//	}
	//	if (!::TranslateAccelerator(window.GetHandle(), acceleartors, &message) && !::IsDialogMessage(window.GetDialog(), &message)) {
	//		::TranslateMessage(&message);
	//		::DispatchMessage(&message);
	//	}
	//}
	//DestroyAcceleratorTable(acceleartors);

	return 0;
}



