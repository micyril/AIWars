#pragma once

#include "robotcomponent.h"
#include "../../mapelement.h"

class RunningGear : public RobotComponent {
private:
	float speed;
	float leftDistanceForMoving;

public:
	RunningGear(float speed);
	virtual void* Execute(std::string command, void *arg);
	virtual void Update(float delta);
};