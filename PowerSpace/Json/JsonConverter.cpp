#include <stdafx.h>
#include <sstream>
#include <JsonConverter.h>
#include <JsonObject.h>
#include <JsonWorker.h>
#include <Stage.h>

IJsonPtr CJsonConverter::ToJsonObject( const CStage &stage )
{
    auto jStage = std::make_shared<CJsonMap>( L"Stage" );
    jStage->objects[L"Objects"] = ToJsonObject( stage.GetObjectsAsVector() );
    jStage->objects[L"ViewPort"] = ToJsonObject( stage.GetViewPort() );
    return std::static_pointer_cast<IJsonObject>(jStage);
}

IJsonPtr CJsonConverter::ToJsonObject( const std::vector<IDrawablePtrConst> &objects )
{
    auto jObjects = std::make_shared<CJsonArray>( L"Objects" );
    for( auto object : objects ) {
        jObjects->objects.push_back( object->ToJson() );
    }
    return std::static_pointer_cast<IJsonObject>(jObjects);
}

IJsonPtr CJsonConverter::ToJsonObject( const CViewport &viewPort )
{
    auto jViewPort = std::make_shared<CJsonMap>( L"ViewPort" );
    jViewPort->objects[L"Scale"] = std::make_shared<CJsonWString>( L"Scale", std::to_wstring( viewPort.GetScale() ) );
    jViewPort->objects[L"Angle"] = std::make_shared<CJsonWString>( L"Angle", std::to_wstring( viewPort.GetAngle() ) );
    jViewPort->objects[L"Zero Location"] = ToJsonObject( viewPort.GetZeroLocation() );
    return std::static_pointer_cast<IJsonObject>(jViewPort);
}

IJsonPtr CJsonConverter::ToJsonObject( const CRectangleObject &rectangle )
{
    auto jRectangle = std::make_shared<CJsonMap>( L"Rectangle" );
    jRectangle->objects[L"Id"] = std::make_shared<CJsonWString>( L"Id", std::wstring( rectangle.GetId() ) );
    jRectangle->objects[L"Color"] = std::make_shared<CJsonWString>( std::to_wstring( rectangle.GetColor() ), L"Color" );
    jRectangle->objects[L"Box"] = ToJsonObject( rectangle.GetContainingBox() );
    jRectangle->objects[L"Scripts"] = ToJsonObject( rectangle.GetScripts( EventType::EventAll ) );
    return std::static_pointer_cast<IJsonObject>(jRectangle);
}

IJsonPtr CJsonConverter::ToJsonObject( const CTextBoxObject &text )
{
    auto jText = std::make_shared<CJsonMap>( L"TextBox" );
    jText->objects[L"Id"] = std::make_shared<CJsonWString>( L"Id", std::wstring( text.GetId() ) );
    jText->objects[L"Color"] = std::make_shared<CJsonWString>( L"Color", std::to_wstring( text.GetColor() ) );
    jText->objects[L"Box"] = ToJsonObject( text.GetContainingBox() );
    jText->objects[L"Scripts"] = ToJsonObject( text.GetScripts( EventType::EventAll ) );
    jText->objects[L"Content"] = std::make_shared<CJsonWString>( L"Content", text.GetContents() );
    return std::static_pointer_cast<IJsonObject>(jText);
}

IJsonPtr CJsonConverter::ToJsonObject( const CEllipseObject &ellipse )
{
    auto jEllipse = std::make_shared<CJsonMap>( L"Ellipse" );
    jEllipse->objects[L"Id"] = std::make_shared<CJsonWString>( L"Id", std::wstring( ellipse.GetId() ) );
    jEllipse->objects[L"Color"] = std::make_shared<CJsonWString>( L"Color", std::to_wstring( ellipse.GetColor() ) );
    jEllipse->objects[L"Box"] = ToJsonObject( ellipse.GetContainingBox() );
    jEllipse->objects[L"Scripts"] = ToJsonObject( ellipse.GetScripts( EventType::EventAll ) );
    return std::static_pointer_cast<IJsonObject>(jEllipse);
}

IJsonPtr CJsonConverter::ToJsonObject( const TBox &box )
{
    auto jBox = std::make_shared<CJsonMap>( L"Containing Box" );
    jBox->objects[L"Left"] = std::make_shared<CJsonWString>( L"Left", std::to_wstring( box.left ) );
    jBox->objects[L"Right"] = std::make_shared<CJsonWString>( L"Right", std::to_wstring( box.right ) );
    jBox->objects[L"Top"] = std::make_shared<CJsonWString>( L"Top", std::to_wstring( box.top ) );
    jBox->objects[L"Bottom"] = std::make_shared<CJsonWString>( L"Bottom", std::to_wstring( box.bottom ) );
    return std::static_pointer_cast<IJsonObject>(jBox);
}

IJsonPtr CJsonConverter::ToJsonObject( const TPoint &point )
{
    auto jPoint = std::make_shared<CJsonMap>( L"Zero Location" );
    jPoint->objects[L"X"] = std::make_shared<CJsonWString>( L"X", std::to_wstring( point.x ) );
    jPoint->objects[L"Y"] = std::make_shared<CJsonWString>( L"Y", std::to_wstring( point.y ) );
    return std::static_pointer_cast<IJsonObject>(jPoint);
}

IJsonPtr CJsonConverter::ToJsonObject( const std::vector<CScript> &objects )
{
    auto jObjects = std::make_shared<CJsonArray>( L"Scripts" );
    for( auto object : objects ) {
        jObjects->objects.push_back( std::make_shared<CJsonWString>( L"Path", object.GetPath() ) );
    }
    return std::static_pointer_cast<IJsonObject>(jObjects);
}

IJsonPtr CJsonConverter::ToJsonObject( const CScript &script )
{
    auto jScript = std::make_shared<CJsonWString>( L"Path", script.GetPath() );
    return std::static_pointer_cast<IJsonObject>(jScript);
}

template<> std::shared_ptr<CRectangleObject> CJsonConverter::fromJson( const IJsonObject& object )
{
    std::wcout << object.name;
    TBox temp = { 0, 0, 50, 50 };
    return std::make_shared<CRectangleObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CEllipseObject> CJsonConverter::fromJson( const IJsonObject& object )
{
    std::wcout << object.name;
    TBox temp = { 0, 0, 50, 50 };
    return std::make_shared<CEllipseObject>( RGB( 100, 90, 80 ), temp );
}

template<> std::shared_ptr<CTextBoxObject> CJsonConverter::fromJson( const IJsonObject& object )
{
    std::wcout << object.name;
    TBox temp = { 0, 0, 50, 50 };
    return std::make_shared<CTextBoxObject>( RGB( 100, 90, 80 ), temp, L"Text" );
}

template<> std::shared_ptr<CViewport> CJsonConverter::fromJson( const IJsonObject& object )
{
    std::wcout << object.name;
    return std::make_shared<CViewport>();
}