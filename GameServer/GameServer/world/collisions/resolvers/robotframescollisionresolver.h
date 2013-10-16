#include "collisionresolver.h"

class RobotFramesCollisionResolver : public CollisionResolver {
public:
	RobotFramesCollisionResolver();
	virtual void Resolve(MapElement *mapElement1, MapElement *mapElement2);
};