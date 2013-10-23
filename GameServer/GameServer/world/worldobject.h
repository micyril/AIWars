#pragma once

#include <list>
#include "mapelement.h"

class WorldObject {
public:
	std::list<MapElement*> mapElements;
	virtual ~WorldObject() {}
	virtual void Update(float delta) = 0;
};