#pragma once
#include "stdafx.h"
#include <string>
#include "Stage.h"
#include "StageObjects.h"
#include <iostream>
typedef std::wstring JSON;
const int LAST_PROP = -1;

class CJsonConverter
{
public:

	template<typename T> 
	T fromJson( const JSON &description );

	JSON toJson( const CStage &stage, int depth = 0 );

	JSON toJson( const CViewport &viewPort, int depth = 0 );
	
	JSON toJson( const CRectangleObject &rectangle, int depth = 0 );
	
	JSON toJson( const CTextBoxObject &text, int depth = 0 );
	
	JSON toJson( const CEllipseObject &ellipse, int depth = 0 );

	JSON toJson( const std::vector<IDrawablePtrConst> &objects, int depth = 0 );

	JSON toJson( const CScript &script, int depth = 0 );

	JSON toJson( const std::vector<CScript> &objects, int depth = 0 );

	JSON toJson( const TBox &box, int depth = 0 );

	JSON toJson( const TPoint &point, int depth = 0 );

private:

	JSON &addElementToDescription( JSON &description, const JSON &element, int depth );

	JSON &addPropertyToDescription( 
		JSON &description, 
		const std::wstring &name, 
		const JSON &prop, 
		int depth, 
		int last=0
	);
	
};


template<> std::shared_ptr<CRectangleObject> CJsonConverter::fromJson( const JSON &description ) 
{ 
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CRectangleObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CEllipseObject> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CEllipseObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CTextBoxObject> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	TBox temp = { 0, 0, 50, 50 };
	return std::make_shared<CTextBoxObject>( RGB( 100, 90, 80 ), temp, L"Text" );
}

template<> std::shared_ptr<CViewport> CJsonConverter::fromJson( const JSON &description )
{
	std::wcout << description;
	return std::make_shared<CViewport>( );
}