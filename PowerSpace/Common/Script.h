#pragma once
#pragma warning(disable : 4592)
#include <JsonObject.h>
#include <unordered_map>
#include <ObjectIdGenerator.h>

using TPath = std::wstring;
using ScriptName = std::wstring;

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
    static std::unordered_map<EventType, IdType> Events;
    explicit CScript( TPath _path );
	// returns path to the file with python script
	const TPath& GetPath() const;
    ScriptName GetName() const;
	std::wstring ToWString() const;
	IJsonPtr ToJson() const;
private:
	TPath path;
    ScriptName name;
};
