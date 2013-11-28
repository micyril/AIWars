#include <algorithm>
#include <cmath>
#include <sstream>
#include "runninggear.h"
#include "../exceptions.h"
#include "../robot.h"

MovementTask::MovementTask(Robot *robot) :
	robot(robot), deltaX(0.0F), deltaY(0.0F), rotation(0.0F) {}

void MovementTask::SetMovement(float deltaX, float deltaY) {
	this->deltaX = deltaX;
	this->deltaY = deltaY;
}

void MovementTask::SetRotation(float rotation) {
	this->rotation = rotation;
}

void MovementTask::Perform() {
	for(auto mapElemIt = robot->mapElements.begin(); mapElemIt != robot->mapElements.end(); mapElemIt++) {
		(*mapElemIt)->Move(deltaX, deltaY);
		(*mapElemIt)->Rotate(rotation);
	}
}

RunningGear::RunningGear(float movingSpeed, float rotationSpeed) : 
	movingSpeed(movingSpeed), rotationSpeed(rotationSpeed), leftDistanceForMoving(0), leftAngleForRotation(0), undoMovementTask(NULL) {
		supportedCommands.push_back("MOV");
		supportedCommands.push_back("ROT");
}

RunningGear::~RunningGear() {
	if(undoMovementTask != NULL)
		delete undoMovementTask;
}

void RunningGear::SetRobot(Robot *robot) {
	RobotComponent::SetRobot(robot);
	undoMovementTask = new MovementTask(robot);
	robot->SetUndoLastMovementTask(undoMovementTask);
}

std::string RunningGear::Execute(const std::string &command, const std::string &arg) {
	//TODO: think out what we should do if we are performing previous command now
	if (command == "MOV") {
		leftDistanceForMoving = (float)atof(arg.c_str());
		return std::string("");  //TODO: use constant
	}
	if (command == "ROT") {
		leftAngleForRotation = (float)atof(arg.c_str());
		return std::string("");  //TODO: use constant
	}
	throw NotSupportedCommandException(command);
}

void RunningGear::Update(float delta) {
	if(robot->frame->GetHealth() > 0) {
		updateMoving(delta);
		updateRotation(delta);
	}
}

void RunningGear::updateMoving(float delta) {
	if(leftDistanceForMoving > 0) {
		float maxDistanceForMoving = movingSpeed * delta;
		float currentDistanceForMoving = std::min<float>(maxDistanceForMoving, leftDistanceForMoving);

		float deltaX = currentDistanceForMoving * cos(robot->frame->rotation);
		float deltaY = currentDistanceForMoving * sin(-robot->frame->rotation);

		for(auto robotMapElemsIt = robot->mapElements.begin(); robotMapElemsIt != robot->mapElements.end(); robotMapElemsIt++)
			(*robotMapElemsIt)->Move(deltaX, deltaY);

		undoMovementTask->SetMovement(-deltaX, -deltaY);

		leftDistanceForMoving -= currentDistanceForMoving;
	}
	else
		undoMovementTask->SetMovement(0.0F, 0.0F);
}

void RunningGear::updateRotation(float delta) {
	if(abs(leftAngleForRotation) > 0) {
		float maxAngleForRotation = rotationSpeed * delta;
		float currentAngleForRotation = std::min<float>(maxAngleForRotation, abs(leftAngleForRotation)) * (leftAngleForRotation > 0? 1.0F : -1.0F);

		for(auto robotMapElemsIt = robot->mapElements.begin(); robotMapElemsIt != robot->mapElements.end(); robotMapElemsIt++)
			(*robotMapElemsIt)->Rotate(currentAngleForRotation);

		undoMovementTask->SetRotation(-currentAngleForRotation);

		leftAngleForRotation -= currentAngleForRotation;
	}
	else
		undoMovementTask->SetRotation(0.0F);
}