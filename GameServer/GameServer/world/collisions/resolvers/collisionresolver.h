#pragma once

#include "../../mapelement.h"

class CollisionResolver {
public:
	virtual ~CollisionResolver() {}
	virtual void Resolve(MapElement *mapElement1, MapElement *mapElement2) = 0;
};