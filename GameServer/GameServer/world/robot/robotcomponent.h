#pragma once

#include <list>
#include "../mapobject.h"
#include "../mapelement.h"

class RobotComponent : public MapObject {
public:
	virtual void* Use(void *arg) = 0;
	virtual std::list<MapElement> GetElements() = 0;
};