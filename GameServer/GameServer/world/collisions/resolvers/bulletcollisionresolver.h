#pragma once

#include "collisionresolver.h"

class BulletBody;
class RobotMapElement;

class BulletCollisionResolver : public CollisionResolver {
private:
	static BulletCollisionResolver *bulletCollisionResolver;

	BulletCollisionResolver();
	void resolve(BulletBody *subject, RobotMapElement *object);
	void resolve(BulletBody *subject, BulletBody *object);

public:
	virtual void Resolve(MapElement *subject, MapElement *object);
	static void Initilize();
};