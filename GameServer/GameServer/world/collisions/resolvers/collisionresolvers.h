#pragma once 

#include "robotcollisionresolver.h"
#include "bulletcollisionresolver.h"

void defineCollisionResolvers() {
	RobotCollisionResolver::Initilize();
	BulletCollisionResolver::Initilize();
}