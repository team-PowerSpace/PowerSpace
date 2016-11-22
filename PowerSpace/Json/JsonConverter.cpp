#include <stdafx.h>
#include <sstream>
#include <JsonConverter.h>
#include <JsonObject.h>
#include <JsonWorker.h>
#include <Stage.h>

#include <JsonTreeVisitor.h>

const std::wstring CJsonConverter::JSON_OBJECT_NAME_ANGLE = L"Angle";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_BOTTOM = L"Bottom";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_BOX = L"Box";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_COLOR = L"Color";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX = L"Containing Box";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_CONTENT = L"Content";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_ELLIPSE = L"Ellipse";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_ID = L"Id";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_LEFT = L"Left";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_OBJECTS = L"Objects";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_PATH = L"Path";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_RECTANGLE = L"Rectangle";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_RIGHT = L"Right";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_SCALE = L"Scale";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_SCRIPTS = L"Scripts";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_STAGE = L"Stage";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_TEXTBOX = L"TextBox";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_TOP = L"Top";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_VIEWPORT = L"ViewPort";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_X = L"X";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_Y = L"Y";
const std::wstring CJsonConverter::JSON_OBJECT_NAME_ZERO_LOCATION = L"Zero Location";

std::shared_ptr<CStage> CJsonConverter::FromJson( const CJsonMap& object )
{
    CStageBuildVisitor visitor;
    visitor.Visit( &object );
    return visitor.GetStage();
}

IJsonPtr CJsonConverter::ToJsonObject( const CStage &stage )
{
    auto jStage = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_STAGE );
    jStage->objects[JSON_OBJECT_NAME_OBJECTS] = ToJsonObject( stage.GetObjectsAsVector() );
    jStage->objects[JSON_OBJECT_NAME_VIEWPORT] = ToJsonObject( stage.GetViewPort() );
    return jStage;
}

IJsonPtr CJsonConverter::ToJsonObject( const std::vector<IDrawablePtrConst> &objects )
{
    auto jObjects = std::make_shared<CJsonArray>( JSON_OBJECT_NAME_OBJECTS );
    for( auto object : objects ) {
        jObjects->objects.push_back( object->ToJson() );
    }
    return jObjects;
}

IJsonPtr CJsonConverter::ToJsonObject( const CViewport &viewPort )
{
    auto jViewPort = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_VIEWPORT );
    jViewPort->objects[JSON_OBJECT_NAME_SCALE] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_SCALE, std::to_wstring( viewPort.GetScale() ) );
    jViewPort->objects[JSON_OBJECT_NAME_ANGLE] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_ANGLE, std::to_wstring( viewPort.GetAngle() ) );
    jViewPort->objects[JSON_OBJECT_NAME_ZERO_LOCATION] = ToJsonObject( viewPort.GetZeroLocation() );
    return jViewPort;
}

IJsonPtr CJsonConverter::ToJsonObject( const CRectangleObject &rectangle )
{
    auto jRectangle = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_RECTANGLE );
    jRectangle->objects[JSON_OBJECT_NAME_ID] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_ID, std::wstring( rectangle.GetId() ) );
    jRectangle->objects[JSON_OBJECT_NAME_COLOR] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_COLOR, std::to_wstring( rectangle.GetColor() ) );
    jRectangle->objects[JSON_OBJECT_NAME_BOX] = ToJsonObject( rectangle.GetContainingBox() );
    jRectangle->objects[JSON_OBJECT_NAME_SCRIPTS] = ToJsonObject( rectangle.GetScripts( EventType::EventAll ) );
    return jRectangle;
}

IJsonPtr CJsonConverter::ToJsonObject( const CTextBoxObject &text )
{
    auto jText = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_TEXTBOX );
    jText->objects[JSON_OBJECT_NAME_ID] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_ID, std::wstring( text.GetId() ) );
    jText->objects[JSON_OBJECT_NAME_COLOR] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_COLOR, std::to_wstring( text.GetColor() ) );
    jText->objects[JSON_OBJECT_NAME_BOX] = ToJsonObject( text.GetContainingBox() );
    jText->objects[JSON_OBJECT_NAME_SCRIPTS] = ToJsonObject( text.GetScripts( EventType::EventAll ) );
    jText->objects[JSON_OBJECT_NAME_CONTENT] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_CONTENT, text.GetContents() );
    return jText;
}

IJsonPtr CJsonConverter::ToJsonObject( const CEllipseObject &ellipse )
{
    auto jEllipse = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_ELLIPSE );
    jEllipse->objects[JSON_OBJECT_NAME_ID] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_ID, std::wstring( ellipse.GetId() ) );
    jEllipse->objects[JSON_OBJECT_NAME_COLOR] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_COLOR, std::to_wstring( ellipse.GetColor() ) );
    jEllipse->objects[JSON_OBJECT_NAME_BOX] = ToJsonObject( ellipse.GetContainingBox() );
    jEllipse->objects[JSON_OBJECT_NAME_SCRIPTS] = ToJsonObject( ellipse.GetScripts( EventType::EventAll ) );
    return jEllipse;
}

IJsonPtr CJsonConverter::ToJsonObject( const TBox &box )
{
    auto jBox = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_CONTAINING_BOX );
    jBox->objects[JSON_OBJECT_NAME_LEFT] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_LEFT, std::to_wstring( box.left ) );
    jBox->objects[JSON_OBJECT_NAME_RIGHT] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_RIGHT, std::to_wstring( box.right ) );
    jBox->objects[JSON_OBJECT_NAME_TOP] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_TOP, std::to_wstring( box.top ) );
    jBox->objects[JSON_OBJECT_NAME_BOTTOM] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_BOTTOM, std::to_wstring( box.bottom ) );
    return jBox;
}

IJsonPtr CJsonConverter::ToJsonObject( const TPoint &point )
{
    auto jPoint = std::make_shared<CJsonMap>( JSON_OBJECT_NAME_ZERO_LOCATION );
    jPoint->objects[JSON_OBJECT_NAME_X] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_X, std::to_wstring( point.x ) );
    jPoint->objects[JSON_OBJECT_NAME_Y] = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_Y, std::to_wstring( point.y ) );
    return jPoint;
}

IJsonPtr CJsonConverter::ToJsonObject( const std::vector<CScript> &objects )
{
    auto jObjects = std::make_shared<CJsonArray>( JSON_OBJECT_NAME_SCRIPTS );
    for( auto object : objects ) {
        jObjects->objects.push_back( std::make_shared<CJsonWString>( JSON_OBJECT_NAME_PATH, object.GetPath() ) );
    }
    return jObjects;
}

IJsonPtr CJsonConverter::ToJsonObject( const CScript &script )
{
    auto jScript = std::make_shared<CJsonWString>( JSON_OBJECT_NAME_PATH, script.GetPath() );
    return jScript;
}
