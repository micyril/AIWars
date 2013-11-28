#include "collisionresolver.h"

class RobotCollisionResolver : public CollisionResolver {
private:
	RobotCollisionResolver();
	static RobotCollisionResolver *robotCollisionResolver;

public:
	virtual void Resolve(MapElement *subject, MapElement *object);
	static void Initilize();
};