#pragma once
#include "StageObjects.h"
#include <iostream>
#include "JsonObject.h"

const bool LAST_PROP = true;

class CStage;

class CJsonConverter
{
public:

	template<typename T>
	static T fromJson( const JSON& description );

	static IJsonPtr ToJsonObject( const CStage& stage );

	static IJsonPtr ToJsonObject( const CViewport& viewPort );

	static IJsonPtr ToJsonObject( const CRectangleObject& rectangle );

	static IJsonPtr ToJsonObject( const CTextBoxObject& text );

	static IJsonPtr ToJsonObject( const CEllipseObject& ellipse );

	static IJsonPtr ToJsonObject( const std::vector<IDrawablePtrConst>& objects );

	static IJsonPtr ToJsonObject( const CScript& script );

	static IJsonPtr ToJsonObject( const std::vector<CScript>& objects );

	static IJsonPtr ToJsonObject( const TBox& box );

	static IJsonPtr ToJsonObject( const TPoint& point );

};