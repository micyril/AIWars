#pragma once

#include "../robotcomponent.h"
#include "../../robot.h"
#include "../../../world.h"
#include "bullet.h"

class GunBarrel : public RobotMapElement {
public:
	GunBarrel(int length, int width);
};

class Gun : public RobotComponent {
private:
	World *world;
	GunBarrel *barrel;
	BulletFactory bulletFactory;
	float reloadingTime;
	float nextShotWaitingTime;
	
public:
	Gun(World *world, int length, int width, float rateOfFire = 2.0F, float bulletsSpeed = 160.0F, int bulletsDamageInfluence = 25);
	virtual ~Gun();
	virtual void SetRobot(Robot *robot);
	virtual std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
};