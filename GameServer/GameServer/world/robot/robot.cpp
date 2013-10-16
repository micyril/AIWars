#include "robot.h"

std::string RobotFrame::type = "RobotFrame";

RobotFrame::RobotFrame(int width, int height, Robot *robot) :
	MapElement(width, height, 0, 0), robot(robot) {}

std::string RobotFrame::GetType() {
	return GetClassType();
}

std::string RobotFrame::GetClassType() {
	return type;
}

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
	return it->second->Execute(command, arg);
}