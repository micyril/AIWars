#include "robot.h"
#include "exceptions.h"

std::string RobotFrame::type = "RobotFrame";

RobotFrame::RobotFrame(int width, int height, float x, float y) :
	MapElement(width, height, x, y) {}

void RobotFrame::SetRobot(Robot *robot) {
	this->robot = robot;
}

std::string RobotFrame::GetType() {
	return GetClassType();
}

std::string RobotFrame::GetClassType() {
	return type;
}

Robot::Robot(RobotFrame *frame, std::map<std::string, RobotComponent*>& commandToRobotComponent) :
	frame(frame), commandToRobotComponent(commandToRobotComponent) {
		frame->SetRobot(this);
		for(std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.begin(); it != commandToRobotComponent.end(); it++)
			it->second->SetRobot(this);
}

void Robot::Update(float delta) {
	for(std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.begin(); it != commandToRobotComponent.end(); it++)
		it->second->Update(delta);
}

void *Robot::Execute(std::string command, void *arg) {
	std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.find(command);
	if (it == commandToRobotComponent.end())
		throw NotSupportedCommandException(command);
	//todo: maybe need to catch exeptions
	return it->second->Execute(command, arg);
}