#include "robot.h"

void Robot::Update(float delta) {
	for(std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.begin(); it != commandToRobotComponent.end(); it++)
		it->second->Update(delta);
}

void *Robot::Execute(std::string command, void *arg) {
	std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.find(command);
	if (it == commandToRobotComponent.end())
	{
		//todo: logging and maybe some other actions
		return NULL;
	}
	//todo: maybe need to catch exeptions
	return it->second->Use(arg);
}