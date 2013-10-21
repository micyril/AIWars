#include <algorithm>
#include <cmath>
#include "runninggear.h"
#include "../exceptions.h"
#include "../robot.h"

RunningGear::RunningGear(float speed) : 
	speed(speed) {}

void* RunningGear::Execute(std::string command, void *arg) {
	//todo: think out what we should do if we are performing previous command now
	if (command == "move") {
		leftDistanceForMoving = *((float *)arg);
		std::string *answer = new std::string("ACK");
		return answer;
	}
	throw NotSupportedCommandException(command);
}

void RunningGear::Update(float delta) {
	if(leftDistanceForMoving > 0) {
		float maxDistanceForMoving = speed * delta;
		float currentDistanceForMoving = std::min<float>(maxDistanceForMoving, leftDistanceForMoving);

		//todo: cooperate coordinates axes choosing between us (server) and java script client

		robot->frame->x += currentDistanceForMoving * cos(robot->frame->rotation);
		robot->frame->y += currentDistanceForMoving * sin(robot->frame->rotation);

		leftDistanceForMoving -= currentDistanceForMoving;
	}
}
