#include "../stdafx.h"
#include "Stage.h"


const std::unordered_map<int, IDrawablePtr>& CStage::GetObjects() const
{
    return objects;
}

std::unordered_map<int, IDrawablePtr>& CStage::GetObjects()
{
    return const_cast<std::unordered_map<int, IDrawablePtr>&>(static_cast<const CStage&>(*this).GetObjects());
}

IDrawablePtrConst CStage::GetObjectById( int objectId ) const
{
    return objects.at( objectId );
}

IDrawablePtr CStage::GetObjectById( int objectId )
{
    return objects.at( objectId );
}
