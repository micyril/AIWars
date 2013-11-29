#include "robot.h"
#include "exceptions.h"

std::string RobotMapElement::type = "RobotMapElement";

RobotMapElement::RobotMapElement(int width, int height, float x, float y, int health) :
	MapElement(width, height, x, y), health(health) {}

void RobotMapElement::SetRobot(Robot *robot) {
	this->robot = robot;
}

Robot * const RobotMapElement::GetRobot() {
	return robot;
}

void RobotMapElement::Damage(int points) {
	health = std::max<int>(health - points, 0);
}

int RobotMapElement::GetHealth() {
	return health;
}

std::string RobotMapElement::GetType() {
	return GetClassType();
}

std::string RobotMapElement::GetClassType() {
	return type;
}

RobotFrame::RobotFrame(int width, int height, float x, float y) :
	RobotMapElement(width, height, x, y) {}

Robot::Robot(RobotFrame *frame, std::list<RobotComponent*>& robotComponents) : frame(frame), robotComponents(robotComponents), undoLastMovementTask(NULL) {
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
	for(auto it = robotComponents.begin(); it != robotComponents.end(); it++)
		(*it)->Update(delta);
}

std::string Robot::Execute(const std::string &command, const std::string &arg) {
	std::map<std::string, RobotComponent*>::iterator it = commandToRobotComponent.find(command);
	if (it == commandToRobotComponent.end())
		throw NotSupportedCommandException(command);
	//todo: maybe need to catch exeptions
	return it->second->Execute(command, arg);
}

void Robot::SetUndoLastMovementTask(Task *undoLastMovementTask) {
	this->undoLastMovementTask = undoLastMovementTask;
}

void Robot::UndoLastMovement() {
	if (undoLastMovementTask != NULL)
		undoLastMovementTask->Perform();
	else
		throw NotDefinedTaskException("to undo last movement");
}