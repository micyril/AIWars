#pragma once

#include <string>
#include <map>
#include "../worldobject.h"
#include "../mapelement.h"
#include "robotcomponent.h"

class Robot : public WorldObject {
private:
	//todo: use safe pointers
	MapElement *frame;
	std::map<std::string, RobotComponent*> commandToRobotComponent;

public:
	void* Execute(std::string command, void *arg);
	void Update(float delta);
};