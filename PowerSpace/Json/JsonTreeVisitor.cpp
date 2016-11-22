#include <stdafx.h>
#include "JsonTreeVisitor.h"

void CStageBuildVisitor::Visit( const CJsonArray* jsonArray )
{
    if( jsonArray->name == CJsonConverter::JSON_OBJECT_NAME_OBJECTS ) {
        for( int i = 0; i < jsonArray->objects.size(); ++i ) {
            jsonArray->objects[i]->Accept( *this );
        }
    } // add scripts here, do not appear in json yet
}

void CStageBuildVisitor::Visit( const CJsonWString* jsonWstring )
{
    if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_SCALE ) {
        viewportScale = std::stod( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_ANGLE ) {
        viewportAngle = std::stod( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_X ) {
        viewportZeroLocation.x = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_Y ) {
        viewportZeroLocation.y = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_TOP ) {
        objBox.top = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_LEFT ) {
        objBox.left = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_RIGHT) {
        objBox.right = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_BOTTOM ) {
        objBox.bottom = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_COLOR ) {
        objColor = std::stoi( jsonWstring->content );
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_ID ) {
        objId = jsonWstring->content;
    } else if( jsonWstring->name == CJsonConverter::JSON_OBJECT_NAME_CONTENT ) {
        objContents = jsonWstring->content;
    }
}

void CStageBuildVisitor::Visit( const CJsonMap* jsonMap )
{
    if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_STAGE ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_OBJECTS )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_VIEWPORT )->Accept( *this );
        stage = std::make_shared<CStage>( objects, viewport );
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_VIEWPORT ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_SCALE )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ANGLE )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ZERO_LOCATION )->Accept( *this );
        viewport = CViewport(viewportScale, viewportAngle, viewportZeroLocation);
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_RECTANGLE ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_COLOR )->Accept( *this );
        //jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ID )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_SCRIPTS )->Accept( *this );
        IDrawablePtr objPtr = std::make_shared<CRectangleObject>( objColor, objBox, objAngle, objScripts );
        objects[objPtr->GetId()] = objPtr;
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_ELLIPSE ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_COLOR )->Accept( *this );
        //jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ID )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_SCRIPTS )->Accept( *this );
        IDrawablePtr objPtr = std::make_shared<CEllipseObject>( objColor, objBox, objAngle, objScripts );
        objects[objPtr->GetId()] = objPtr;
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_TEXTBOX ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_COLOR )->Accept( *this );
        //jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ID )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_SCRIPTS )->Accept( *this );
        IDrawablePtr objPtr = std::make_shared<CTextBoxObject>( objColor, objBox, objAngle, objContents, objScripts );
        objects[objPtr->GetId()] = objPtr;
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_TOP )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_LEFT )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_RIGHT )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_BOTTOM )->Accept( *this );
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_ZERO_LOCATION ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_X )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_Y )->Accept( *this );
    }
}

std::shared_ptr<CStage> CStageBuildVisitor::GetStage()
{
    return stage;
}
