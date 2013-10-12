#pragma once

#include <list>
#include "worldobject.h"

class World {
private:
	//todo: borders
	//todo: use safe pointers
	std::list<WorldObject*> objects;
public:
	void Update(float delta);
};