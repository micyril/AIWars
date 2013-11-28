#pragma once

#include "robotcomponent.h"
#include "../../mapelement.h"
#include "../task.h"

class MovementTask : public Task {
private:
	float deltaX, deltaY;
	float rotation;
	Robot *robot;

public:
	MovementTask(Robot *robot);
	void SetMovement(float deltaX, float deltaY);
	void SetRotation(float rotation);
	virtual void Perform();
};

class RunningGear : public RobotComponent {
private:
	float movingSpeed;
	float rotationSpeed;
	float leftDistanceForMoving;
	float leftAngleForRotation;
	MovementTask* undoMovementTask;

	void updateMoving(float delta);
	void updateRotation(float delta);

public:
	RunningGear(float movingSpeed, float rotationSpeed);
	virtual ~RunningGear();
	virtual void SetRobot(Robot *robot);
	virtual std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
};