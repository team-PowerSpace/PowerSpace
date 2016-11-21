#pragma once
#include <StageObjects.h>
#include <iostream>
#include <JsonObject.h>

const bool LAST_PROP = true;

class CStage;

class CJsonConverter
{
public:

	//template<typename T>
	//static T FromJson( const CJsonMap& object);

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

    static const std::wstring JSON_OBJECT_NAME_ANGLE;
    static const std::wstring JSON_OBJECT_NAME_BOTTOM;
    static const std::wstring JSON_OBJECT_NAME_BOX;
    static const std::wstring JSON_OBJECT_NAME_COLOR;
    static const std::wstring JSON_OBJECT_NAME_CONTAINING_BOX;
    static const std::wstring JSON_OBJECT_NAME_CONTENT;
    static const std::wstring JSON_OBJECT_NAME_ELLIPSE;
    static const std::wstring JSON_OBJECT_NAME_ID;
    static const std::wstring JSON_OBJECT_NAME_LEFT;
    static const std::wstring JSON_OBJECT_NAME_OBJECTS;
    static const std::wstring JSON_OBJECT_NAME_PATH;
    static const std::wstring JSON_OBJECT_NAME_RECTANGLE;
    static const std::wstring JSON_OBJECT_NAME_RIGHT;
    static const std::wstring JSON_OBJECT_NAME_SCALE;
    static const std::wstring JSON_OBJECT_NAME_SCRIPTS;
    static const std::wstring JSON_OBJECT_NAME_STAGE;
    static const std::wstring JSON_OBJECT_NAME_TEXT;
    static const std::wstring JSON_OBJECT_NAME_TEXTBOX;
    static const std::wstring JSON_OBJECT_NAME_TOP;
    static const std::wstring JSON_OBJECT_NAME_VIEWPORT;
    static const std::wstring JSON_OBJECT_NAME_X;
    static const std::wstring JSON_OBJECT_NAME_Y;
    static const std::wstring JSON_OBJECT_NAME_ZERO_LOCATION;
};