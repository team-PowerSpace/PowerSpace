#pragma once
#include <JsonObject.h>

using TPath = std::wstring;

// this enum class lists all supported types of events
enum class EventType
{
    EventClick,
    EventTick,
    EventAll
};

// this class wraps the path to the file with python script
class CScript
{
public:
    explicit CScript( TPath _path );

    // returns path to the file with python script
    const TPath& GetPath() const;
    std::wstring ToWString() const;
    IJsonPtr ToJson() const;
private:
    TPath path;
};
