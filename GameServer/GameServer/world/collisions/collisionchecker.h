#pragma once

#include "../mapelement.h"

class CollisionChecker {
public:
	virtual bool Check(MapElement *mapElement1, MapElement *mapElement2) = 0;
};