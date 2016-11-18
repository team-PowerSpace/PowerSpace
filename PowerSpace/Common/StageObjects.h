#pragma once
#include <Canvas.h>
#include <Viewport.h>

class CDrawable;

// object of the stage, represents a rectangle
class CRectangleObject : public CDrawable
{
public:
	explicit CRectangleObject( COLORREF _color, TBox _box );
	void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring ToWString() const;
	IJsonPtr ToJson() const;
};

// object of the stage, represents an ellipse
class CEllipseObject : public CDrawable
{
public:
	explicit CEllipseObject( COLORREF _color, TBox _box );
	void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring ToWString() const;
	IJsonPtr ToJson() const;

};

// object of the stage, represents a box of text
class CTextBoxObject : public CDrawable
{
public:
	explicit CTextBoxObject( COLORREF _color, TBox _box, const std::wstring& _contents );
	void Draw( HDC hdc, const CViewport& viewport, const CCanvas& canvas ) const;
	std::wstring ToWString() const;
	IJsonPtr ToJson() const;
	std::wstring GetContents() const;
	std::string GetText() const;
	unsigned int GetFontSize() const;
	unsigned long int GetFontColor() const;
	void SetContents( const std::wstring &_contents );
	void SetFont( HFONT newFont );
	void SetFontColor( COLORREF color_ );
private:
	// text, that is stored in the text box
	std::wstring contents;
	// font of the text
	HFONT font;
	COLORREF fontColor;
	unsigned int fontSize;
};
