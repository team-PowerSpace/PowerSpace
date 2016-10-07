#pragma once
#include "EditorRenderingWindow.h"
#include "Stage.h"
#include "Resource.h"

// This class in made to divide GUI and busines-logic
// CRenderingWindow is GUI, work with screen coordinates
// CEditorWindow is business-logic (mostly), work with model coordinates

class CEditorWindow :
	public CEditorRenderingWindow {
public:
	CEditorWindow();

	virtual ~CEditorWindow() override;

	std::shared_ptr<CStage>& GetStage();

	void SetStage( std::shared_ptr<CStage> stage_ );

	static bool RegisterClass();

	bool Create();

protected:
	virtual void DrawContent( HDC paintDC, const int width, const int height ) override;


private:
	static const wchar_t* ClassName;
	std::shared_ptr<CStage> stage;
};

