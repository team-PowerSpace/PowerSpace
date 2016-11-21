#include <stdafx.h>
#include "JsonTreeVisitor.h"

void CStageBuildVisitor::Visit( const CJsonArray* jsonArray )
{
    if( jsonArray->name == CJsonConverter::JSON_OBJECT_NAME_OBJECTS ) {
        for( int i = 0; i < jsonArray->objects.size(); ++i ) {
            jsonArray->objects[i]->Accept( *this );
        }
    }
}

void CStageBuildVisitor::Visit( const CJsonWString* jsonWstring )
{
    
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
        viewport = CViewport(scale, angle, zeroLocation);
    } else if( jsonMap->name == CJsonConverter::JSON_OBJECT_NAME_RECTANGLE ) {
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_CONTAINING_BOX )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_COLOR )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_ID )->Accept( *this );
        jsonMap->objects.at( CJsonConverter::JSON_OBJECT_NAME_SCRIPTS )->Accept( *this );
        IDrawablePtr rectPtr = std::make_shared<CRectangleObject>( color, box, );
    }
}

std::shared_ptr<CStage> CStageBuildVisitor::GetStage()
{
    return stage;
}
