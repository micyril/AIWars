#pragma once

#include "../../mapelement.h"

class CollisionResolver {
public:
	virtual ~CollisionResolver() {}
	virtual void Resolve(MapElement *subject, MapElement *object) = 0;
};