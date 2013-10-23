#include <algorithm>
#include <cmath>
#include "runninggear.h"
#include "../exceptions.h"
#include "../robot.h"

RunningGear::RunningGear(float movingSpeed, float rotationSpeed) : 
	movingSpeed(movingSpeed), rotationSpeed(rotationSpeed) {
		supportedCommands.push_back("move");
		supportedCommands.push_back("rotate");
}

void* RunningGear::Execute(std::string command, void *arg) {
	//todo: think out what we should do if we are performing previous command now
	if (command == "move") {
		leftDistanceForMoving = *((float *)arg);
		std::string *answer = new std::string("ACK");
		return answer;
	}
	if (command == "rotate") {
		leftAngleForRotation = *((float *)arg);
		std::string *answer = new std::string("ACK");
		return answer;
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