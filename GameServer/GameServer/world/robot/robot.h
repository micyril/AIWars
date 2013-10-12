#pragma once

#include <string>
#include <map>
#include "../mapobject.h"
#include "../mapelement.h"
#include "robotcomponent.h"

class Robot : public MapObject {
private:
	//todo: use safe pointers
	MapElement *frame;
	std::map<std::string, RobotComponent*> commandToRobotComponent;

public:
	void* Execute(std::string command, void *arg);
	void Update(float delta);
};