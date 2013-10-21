#pragma once

#include <list>
#include "../../worldobject.h"
#include "../../mapelement.h"

class Robot;

class RobotComponent : public WorldObject {
protected:
	Robot *robot;

public:
	std::list<MapElement *> mapelements;

	virtual ~RobotComponent() {}
	virtual void* Execute(std::string command, void *arg) = 0;
	void SetRobot(Robot *robot) { this->robot = robot; }
};