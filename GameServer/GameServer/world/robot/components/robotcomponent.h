#pragma once

#include <list>
#include "../../worldobject.h"
#include "../../mapelement.h"

class Robot;

class RobotComponent : public WorldObject {
protected:
	Robot *robot;

public:
	std::list<std::string> supportedCommands;

	virtual ~RobotComponent() {}
	virtual std::string Execute(const std::string &command, const std::string &arg) = 0;
	void SetRobot(Robot *robot) { 
		this->robot = robot; 
	}
};