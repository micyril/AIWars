#pragma once

#include <string>
#include <map>
#include "../worldobject.h"
#include "../mapelement.h"
#include "components/robotcomponent.h"

class Robot;

class RobotFrame : public MapElement
{
private:
	Robot *robot;
	static std::string type;

public:
	RobotFrame(int width, int height, float x = 0, float y = 0);
	void SetRobot(Robot *robot);
	virtual std::string GetType();
	static std::string GetClassType();
};

class Robot : public WorldObject {
private:
	//todo: use safe pointers
	std::map<std::string, RobotComponent*> commandToRobotComponent;

public:
	Robot(RobotFrame *frame, std::list<RobotComponent*>& robotComponents);
	RobotFrame *frame;

	void* Execute(std::string command, void *arg);
	void Update(float delta);
};