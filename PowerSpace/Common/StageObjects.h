#pragma once
#include "Canvas.h"
#include "Drawable.h"
#include "Viewport.h"

// object of the stage, represents a rectangle
class CRectangleObject : public CDrawable
{
public:
    explicit CRectangleObject( COLORREF _color, TBox _box );
    void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring toWString( ) const;
};

// object of the stage, represents an ellipse
class CEllipseObject : public CDrawable
{
public:
    explicit CEllipseObject( COLORREF _color, TBox _box );
    void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring toWString( ) const;
};

// object of the stage, represents a box of text
class CTextBoxObject : public CDrawable
{
public:
    explicit CTextBoxObject( COLORREF _color, TBox _box, const std::wstring& _contents );
    void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring toWString( ) const;
	std::wstring GetContents( ) const;
	void SetContents( const std::wstring &_contents );
private:
    // text, that is stored in the text box
    std::wstring contents;
};
