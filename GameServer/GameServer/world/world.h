#pragma once

#include <list>
#include "mapobject.h"

class World {
private:
	//todo: borders
	//todo: use safe pointers
	std::list<MapObject*> objects;
public:
	void Update(float delta);
};