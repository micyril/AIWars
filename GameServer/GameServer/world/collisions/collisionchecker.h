#pragma once

#include "../mapelement.h"

class CollisionChecker {
public:
	bool static Check(MapElement *mapElement1, MapElement *mapElement2);
};