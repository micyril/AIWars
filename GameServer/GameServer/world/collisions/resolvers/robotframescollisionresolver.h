#include "collisionresolver.h"

class RobotFramesCollisionResolver : public CollisionResolver {
private:
	RobotFramesCollisionResolver();
	static RobotFramesCollisionResolver *robotFramesCollisionResolver;

public:
	virtual void Resolve(MapElement *mapElement1, MapElement *mapElement2);
	static void Initilize();
};