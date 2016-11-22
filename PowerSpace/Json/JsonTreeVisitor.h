#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Stage.h>
#include <JsonObject.h>
#include <JsonConverter.h>

class IJsonTreeVisitor
{
public:
    virtual ~IJsonTreeVisitor() {}
    virtual void Visit( const CJsonArray* jsonArray ) = 0;
    virtual void Visit( const CJsonWString* jsonWstring) = 0;
    virtual void Visit( const CJsonMap* jsonMap ) = 0;
};

class CStageBuildVisitor : IJsonTreeVisitor
{
public:
    ~CStageBuildVisitor() {};
    void Visit( const CJsonArray* jsonArray ) override;
    void Visit( const CJsonWString* jsonWstring ) override;
    void Visit( const CJsonMap* jsonMap ) override;

    std::shared_ptr<CStage> GetStage();
private:
    std::shared_ptr<CStage> stage;
    
    std::unordered_map<IdType, IDrawablePtr> objects;

    CViewport viewport;

    double viewportScale, viewportAngle;
    TPoint viewportZeroLocation;

    COLORREF objColor;
    TBox objBox;
    double objAngle = 0; // doesn't appear in Json, must be added later
    std::wstring objContents;
    IdType objId;
    std::unordered_map<EventType, std::vector<CScript>> objScripts;
};

