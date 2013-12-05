#pragma once

#include "../robotcomponent.h"
#include "../../../world.h"

class VisualScanner : public RobotComponent {
private:
	World *world;
	float viewAngle;
	int viewDistance;
	int scanRectangleHeight;

public:
	VisualScanner(World *world, float viewAngle = 1.0F, float viewDistance = 120.0F);
	virtual void SetRobot(Robot *robot);
	virtual std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
};