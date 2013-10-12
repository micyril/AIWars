#pragma once

#include <list>
#include "../worldobject.h"
#include "../mapelement.h"

class RobotComponent : public WorldObject {
public:
	virtual void* Use(void *arg) = 0;
	virtual std::list<MapElement> GetElements() = 0;
};