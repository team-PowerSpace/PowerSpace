#include "stdafx.h"
#include "Canvas.h"

void CCanvas::DrawRectangle( HDC hdc, IDrawablePtrConst rectangle ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( hdc );
    UNREFERENCED_PARAMETER( rectangle );
}

void CCanvas::DrawCircle( HDC hdc, IDrawablePtrConst circle ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( hdc );
    UNREFERENCED_PARAMETER( circle );
}

void CCanvas::DrawText( HDC hdc, IDrawablePtrConst text ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( hdc );
    UNREFERENCED_PARAMETER( text );
}

void CCanvas::DrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objects ) const
{
    for( const IDrawablePtrConst& object : objects ) {
        object->Draw( hdc );
    }
}

std::vector<int> CCanvas::ClipObjects( const std::vector<IDrawablePtrConst>& objects ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( objects );
    return std::vector<int>();
}

void CCanvas::ClipAndDrawObjects( HDC hdc, const std::vector<IDrawablePtrConst>& objects ) const
{
    std::vector<int> objectsClippedIndices = ClipObjects( objects );
    std::vector<IDrawablePtrConst> objectsClipped;
    objectsClipped.reserve( objectsClippedIndices.size() );
    for( int i : objectsClippedIndices ) {
        objectsClipped.push_back( objects[i] );
    }
    DrawObjects( hdc, objectsClipped );
}

RECT CCanvas::ConvertToCoordinates( TPosition logicalPosition ) const
{
    // TODO: implement this method
    UNREFERENCED_PARAMETER( logicalPosition );
    RECT rect = {};
    return rect;
}
