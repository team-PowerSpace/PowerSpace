#include "../stdafx.h"
#include "Stage.h"


const std::unordered_map<int, IDrawable>& CStage::GetObjects() const
{
    return objects;
}

std::unordered_map<int, IDrawable> CStage::GetObjects()
{
    return const_cast<std::unordered_map<int, IDrawable>&>(static_cast<const CStage&>(*this).GetObjects());
}

const IDrawable& CStage::GetObject( int objectId ) const
{
    return objects.at( objectId );
}

IDrawable& CStage::GetObject( int objectId )
{
    return const_cast<IDrawable&>(static_cast<const CStage&>(*this).GetObject( objectId ));
}
