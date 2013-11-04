#pragma once

#include "../mapelement.h"

class CollisionChecker {
public:
	static bool Check(MapElement *mapElement1, MapElement *mapElement2);
};