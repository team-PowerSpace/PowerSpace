#pragma once
#include <JsonObject.h>
#include <stack>

class CJsonWorker
{
public:

    static JSON& AddRowToDescription(
        JSON& description,
        const JSON &row,
        int depth
        );

    static JSON& AddElementToDescription(
        JSON& description,
        const JSON &element,
        int depth,
        std::wstring sep = L""
        );

    static int OpenTag(
        JSON& description,
        const std::wstring& tag,
        int depth
        );

    static int CloseTag(
        JSON& description,
        const std::wstring& tag,
        int depth
        );

    static std::vector<IJsonPtr> ReadObjects( const JSON& description );

    static JsonObjectDescription GetNextObjectDescription( const JSON& description, JsonPosition position = 0 );

    static const JsonName EMPTY_NAME;
    static const JsonObjectBody EMPTY_BODY;
    
    static const std::unordered_map<JsonTag, JsonObjectType> typeFromTag;

private:
    static void updateOperatorsStack( std::stack<JsonTag>& operators, JsonTag newTag );

};