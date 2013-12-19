#include "robotcollisionresolver.h"
#include "../collisionresolvermaster.h"
#include "../../robot/robot.h"

RobotCollisionResolver* RobotCollisionResolver::robotCollisionResolver = NULL;

RobotCollisionResolver::RobotCollisionResolver() {
	CollisionResolverMaster::AddNewCollisionResolver(RobotMapElement::GetClassType(), RobotMapElement::GetClassType(), this);
}

void RobotCollisionResolver::Resolve(MapElement *updatedElement, MapElement *collidedElement) {
	RobotMapElement *rmeCollidedElement = (RobotMapElement *)collidedElement;
	RobotMapElement *rmeUpdatedElement = (RobotMapElement *)updatedElement;

	if (rmeCollidedElement->GetRobot() != rmeUpdatedElement->GetRobot())
		rmeUpdatedElement->GetRobot()->UndoLastMovement();
}

void RobotCollisionResolver::Initilize() {
	if(robotCollisionResolver == NULL)
		robotCollisionResolver = new RobotCollisionResolver();
}