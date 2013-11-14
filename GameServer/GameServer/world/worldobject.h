#pragma once

#include <list>
#include "mapelement.h"

class WorldObject {
public:
	std::list<MapElement*> mapElements;
	virtual ~WorldObject() {
		for(auto mapElemIt = mapElements.begin(); mapElemIt != mapElements.end(); mapElemIt++)
			delete *mapElemIt;
	}
	virtual void Update(float delta) = 0;
};