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

	static const float piHalf;  // = 1.57...

public:
	RunningGear(float movingSpeed, float rotationSpeed);
	virtual std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
};