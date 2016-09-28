#pragma once

using TPath = std::wstring;

// this enum class lists all supported types of events
enum class EventType
{
    EventClick,
    EventTick
};

// this class wraps the path to the file with python script
class CScript
{
public:
    explicit CScript( TPath _path );

    // returns path to the file with python script
    const TPath& GetPath() const;
private:
    TPath path;
};
