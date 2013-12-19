#pragma once

#include "collisionresolver.h"

class BulletBody;
class RobotMapElement;

class BulletCollisionResolver : public CollisionResolver {
private:
	static BulletCollisionResolver *bulletCollisionResolver;

	BulletCollisionResolver();
	void resolve(BulletBody *updatedElement, RobotMapElement *collidedElement);
	void resolve(BulletBody *updatedElement, BulletBody *collidedElement);

public:
	virtual void Resolve(MapElement *updatedElement, MapElement *collidedElement);
	static void Initilize();
};