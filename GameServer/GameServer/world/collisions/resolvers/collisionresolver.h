#pragma once

#include "../../mapelement.h"

class CollisionResolver {
public:
	virtual ~CollisionResolver() {}
	virtual void Resolve(MapElement *updatedElement, MapElement *collidedElement) = 0;
};