#include <algorithm>
#include <cmath>
#include <sstream>
#include "runninggear.h"
#include "../exceptions.h"
#include "../robot.h"

//TODO: do not use that
template<typename T>
T convertFromString(const std::string &s) {
	T result;
	std::stringstream stream(s);
	stream >> result;
	return result;
}

RunningGear::RunningGear(float movingSpeed, float rotationSpeed) : 
	movingSpeed(movingSpeed), rotationSpeed(rotationSpeed) {
		supportedCommands.push_back("MOV");
		supportedCommands.push_back("ROT");
}

std::string RunningGear::Execute(const std::string &command, const std::string &arg) {
	//TODO: think out what we should do if we are performing previous command now
	if (command == "MOV") {
		leftDistanceForMoving = convertFromString<float>(arg);
		return std::string("");  //TODO: use constant
	}
	if (command == "ROT") {
		leftAngleForRotation = convertFromString<float>(arg);
		return std::string("");  //TODO: use constant
	}
	throw NotSupportedCommandException(command);
}

void RunningGear::Update(float delta) {
	updateMoving(delta);
	updateRotation(delta);
}

void RunningGear::updateMoving(float delta) {
	if(leftDistanceForMoving > 0) {
		float maxDistanceForMoving = movingSpeed * delta;
		float currentDistanceForMoving = std::min<float>(maxDistanceForMoving, leftDistanceForMoving);

		//todo: cooperate coordinates axes choosing between us (server) and java script client

		float deltaX = currentDistanceForMoving * cos(robot->frame->rotation);
		float deltaY = currentDistanceForMoving * sin(robot->frame->rotation);
		robot->frame->x += deltaX;
		robot->frame->y += deltaY;
		robot->frame->rotationCenterX += deltaX;
		robot->frame->rotationCenterY += deltaY;

		leftDistanceForMoving -= currentDistanceForMoving;
	}
}

void RunningGear::updateRotation(float delta) {
	if(abs(leftAngleForRotation) > 0) {
		float maxAngleForRotation = rotationSpeed * delta;
		float currentAngleForRotation = std::min<float>(maxAngleForRotation, abs(leftAngleForRotation)) * (leftAngleForRotation > 0? 1.0F : -1.0F);

		robot->frame->rotation += currentAngleForRotation;
		leftAngleForRotation -= currentAngleForRotation;
	}
}