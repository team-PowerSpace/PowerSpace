#pragma once
#include <EditorRenderingWindow.h>
#include <Stage.h>
#include "resource.h"

// This class in made to divide GUI and business-logic
// CRenderingWindow is GUI, work with screen coordinates
// CEditorWindow is business-logic (mostly), work with model coordinates

class CEditorWindow :
	public CEditorRenderingWindow
{
public:
	CEditorWindow();

	virtual ~CEditorWindow() override;

	std::shared_ptr<CStage>& GetStage();

	void SetStage( std::shared_ptr<CStage> stage_ );

	static bool RegisterClass();

	bool Create( HWND hWndParent );

protected:
	// Унаследовано через CEditorRenderingWindow
	virtual void DrawContent( HDC paintDC, const int width, const int height ) override;
	virtual void MoveCanvas( const POINT& point ) override;
	virtual void MoveDrawableObject( const IdType& id, const RECT & newSize ) override;
	virtual void Scaling( const int direction ) override;
	virtual void SelectDrawableObject( const IdType& id ) override;
	virtual void RotateDrawableObject( const IdType& id, const double newAngle ) override;


private:
	static const wchar_t* ClassName;
	std::shared_ptr<CStage> stage;
	static const double scalingFactor;
};
