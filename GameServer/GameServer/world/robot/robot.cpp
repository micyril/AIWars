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

Robot::Robot(RobotFrame *frame, std::list<RobotComponent*>& robotComponents) : frame(frame) {
	frame->SetRobot(this);
	mapElements.push_back(frame);
	for(std::list<RobotComponent*>::iterator it = robotComponents.begin(); it != robotComponents.end(); it++) {
		for(std::list<std::string>::iterator commandIter = (*it)->supportedCommands.begin(); commandIter != (*it)->supportedCommands.end(); commandIter++)
			commandToRobotComponent[*commandIter] = *it;
		for(std::list<MapElement*>::iterator mapElemIt = (*it)->mapElements.begin(); mapElemIt != (*it)->mapElements.end(); mapElemIt++)
			mapElements.push_back(*mapElemIt);
		(*it)->SetRobot(this);
	}
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