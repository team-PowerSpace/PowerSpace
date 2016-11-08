#include <stdafx.h>
#include "EditorWindow.h"
#include "Editor.h"


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
	windowClassInforamtion.style = CS_HREDRAW | CS_VREDRAW;
	windowClassInforamtion.lpfnWndProc = WindowProc;
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

void CEditorWindow::DrawContent( HDC paintDC, const int width, const int height )
{
	std::unordered_map<IdType, IDrawablePtr>& objects = stage->GetObjects();
	stage->ClipAndDrawObjects( paintDC, stage->GetObjectsAsVector() );
	CViewport& viewport = stage->GetViewPort();
	for( auto it = objects.begin(); it != objects.end(); ++it ) {
		DrawSizeableRectangle( paintDC, viewport.ConvertToScreenCoordinates( it->second->GetContainingBox() ), it->first );
	}

	UNREFERENCED_PARAMETER( width );
	UNREFERENCED_PARAMETER( height );
}

void CEditorWindow::MoveCanvas( const POINT& point )
{
	CViewport& viewport = stage->GetViewPort();
	viewport.SetZeroLocation( point );
}

void CEditorWindow::MoveRectangle( const IdType& id, const RECT & newSize )
{
	stage->GetObjectById( id )->SetContainingBox( stage->GetViewPort().ConvertToModelCoordinates( newSize ) );
}

void CEditorWindow::Scaling( const int direction )
{
	CViewport& viewport = stage->GetViewPort();
	viewport.SetScale( viewport.GetScale() * pow( scalingFactor, direction ) );
}

void CEditorWindow::SelectRectangle( const IdType& id )
{
	CEditor::GetWindowByHandle( GetParent( GetHandle() ) )->SetActiveId( id );
}

bool CEditorWindow::Create( HWND hWndParent )
{
	return CEditorRenderingWindow::Create(hWndParent, ClassName );
}

const wchar_t* CEditorWindow::ClassName = L"CEditorWindow";

const float CEditorWindow::scalingFactor = 1.2f;
