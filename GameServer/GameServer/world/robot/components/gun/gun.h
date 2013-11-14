#pragma once

#include "../robotcomponent.h"
#include "../../../world.h"
#include "bullet.h"

class GunBarrel : public MapElement {
private:
	static std::string type;

public:
	GunBarrel(int length, int width);
	virtual std::string GetType();
	static std::string GetClassType();
};

class Gun : public RobotComponent {
private:
	World *world;
	GunBarrel *barrel;
	BulletFactory bulletFactory;  //todo: init
	float reloadingTime;
	float nextShotWaitingTime;
	
public:
	Gun(World *world, int length, int width, float rateOfFire = 2.0F, float bulletsSpeed = 160.0F);
	virtual ~Gun();
	virtual void SetRobot(Robot *robot);
	virtual std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
};