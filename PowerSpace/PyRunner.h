#pragma once
#include <string>
#include "ObjectIdGenerator.h"
#include "Script.h"
#include "Drawable.h"
#include "CDrawableBuilder.h"

class PyRunner {
public:
	static void run(IdType object, EventType type, std::vector<IdType> scripts);
	static void update( std::shared_ptr<CDrawableBuilder> pObject, std::shared_ptr<IDrawable> cObject );
};