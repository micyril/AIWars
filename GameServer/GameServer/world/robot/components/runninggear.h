#pragma once

#include "robotcomponent.h"
#include "../../mapelement.h"

class RunningGear : public RobotComponent {
private:
	float movingSpeed;
	float rotationSpeed;
	float leftDistanceForMoving;
	float leftAngleForRotation;

	void updateMoving(float delta);
	void updateRotation(float delta);

public:
	RunningGear(float movingSpeed, float rotationSpeed);
	virtual void* Execute(std::string command, void *arg);
	virtual void Update(float delta);
};