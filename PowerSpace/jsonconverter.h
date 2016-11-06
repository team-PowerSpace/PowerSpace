#pragma once
#include "stdafx.h"
#include "Stage.h"
#include "StageObjects.h"
#include <iostream>
typedef std::wstring JSON;
const bool LAST_PROP = true;

class CJsonConverter
{
public:

	template<typename T> 
	static T fromJson( const JSON &description );

	static JSON toJson( const CStage &stage, int depth = 0 );

	static JSON toJson( const CViewport &viewPort, int depth = 0 );
	
	static JSON toJson( const CRectangleObject &rectangle, int depth = 0 );
	
	static JSON toJson( const CTextBoxObject &text, int depth = 0 );
	
	static JSON toJson( const CEllipseObject &ellipse, int depth = 0 );

	static JSON toJson( const std::vector<IDrawablePtrConst> &objects, int depth = 0 );

	static JSON toJson( const CScript &script, int depth = 0 );

	static JSON toJson( const std::vector<CScript> &objects, int depth = 0 );

	static JSON toJson( const TBox &box, int depth = 0 );

	static JSON toJson( const TPoint &point, int depth = 0 );

private:

	static JSON &addRowToDescription(
		JSON &description,
		const JSON &row,
		int depth
	);

	static JSON &addElementToDescription( 
		JSON &description, 
		const JSON &element, 
		int depth,
		std::wstring sep = L""
	);

	static JSON &addSimplePropertyToDescription(
		JSON &description, 
		const std::wstring &name, 
		const JSON &prop, 
		int depth, 
		bool last = false
	);

	static JSON &addCompoundPropertyToDescription(
		JSON &description,
		const std::wstring &name,
		const JSON &prop,
		int depth,
		bool last = false
	);

	static int openTag(
		JSON &description,
		const std::wstring &tag,
		int depth
	);

	static int closeTag(
		JSON &description,
		const std::wstring &tag,
		int depth
	);
	
};