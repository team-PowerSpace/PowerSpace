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
    //CStageBuildVisitor() : jsonNameStack( 1, CJsonConverter::JSON_OBJECT_NAME_STAGE ) {}
    ~CStageBuildVisitor() {};
    void Visit( const CJsonArray* jsonArray ) override;
    void Visit( const CJsonWString* jsonWstring ) override;
    void Visit( const CJsonMap* jsonMap ) override;

    std::shared_ptr<CStage> GetStage();
private:
    std::unordered_map<IdType, IDrawablePtr> objects;
    CViewport viewport;
    double scale;
    double angle;
    TPoint zeroLocation;
    std::shared_ptr<CStage> stage;
};

