#pragma once

#include <list>
#include "../worldobject.h"
#include "../mapelement.h"

class RobotComponent : public WorldObject {
public:
	virtual ~RobotComponent() {}
	virtual void* Execute(std::string command, void *arg) = 0;
	virtual std::list<MapElement *> GetElements() = 0;
};