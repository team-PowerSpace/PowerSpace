#include "stdafx.h"
#include "EditorWindow.h"


CEditorWindow::CEditorWindow()
{
}


CEditorWindow::~CEditorWindow()
{
}

std::shared_ptr<CStage>& CEditorWindow::GetStage()
{
	return stage;
}

void CEditorWindow::SetStage( std::shared_ptr<CStage> stage_ )
{
	stage = stage_;
}

bool CEditorWindow::RegisterClass()
{
	HBRUSH backgroundBrush = reinterpret_cast<HBRUSH> (::GetStockObject( WHITE_BRUSH ));
	HMODULE instance = GetModuleHandleW( nullptr );

	WNDCLASSEX windowClassInforamtion;
	windowClassInforamtion.cbSize = sizeof( WNDCLASSEX );
	windowClassInforamtion.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassInforamtion.lpfnWndProc = windowProc;
	windowClassInforamtion.cbClsExtra = 0;
	windowClassInforamtion.cbWndExtra = 2 * sizeof( LONG_PTR );
	windowClassInforamtion.hInstance = instance;
	windowClassInforamtion.hIcon = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );
	windowClassInforamtion.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClassInforamtion.hbrBackground = backgroundBrush;
	windowClassInforamtion.lpszMenuName = 0;
	windowClassInforamtion.lpszClassName = ClassName;
	windowClassInforamtion.hIconSm = LoadIcon( instance, MAKEINTRESOURCE( IDI_SMALL ) );

	return (::RegisterClassEx( &windowClassInforamtion ) == 0 ? false : true);
}

void CEditorWindow::drawContent( HDC paintDC, const int width, const int height )
{
	UNREFERENCED_PARAMETER( paintDC );
	UNREFERENCED_PARAMETER( width );
	UNREFERENCED_PARAMETER( height );
}

bool CEditorWindow::Create()
{
	return CRenderingWindow::Create( ClassName );
}

const wchar_t* CEditorWindow::ClassName = L"CEditorWindow";
