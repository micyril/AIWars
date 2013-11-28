#include "robotcollisionresolver.h"
#include "../collisionresolvermaster.h"
#include "../../robot/robot.h"

RobotCollisionResolver* RobotCollisionResolver::robotCollisionResolver = NULL;

RobotCollisionResolver::RobotCollisionResolver() {
	CollisionResolverMaster::AddNewCollisionResolver(RobotMapElement::GetClassType(), RobotMapElement::GetClassType(), this);
}

void RobotCollisionResolver::Resolve(MapElement *subject, MapElement *object) {
	RobotMapElement *rmeSubject = (RobotMapElement *)subject;
	RobotMapElement *rmeObject = (RobotMapElement *)object;

	if (rmeSubject->GetRobot() != rmeObject->GetRobot())
		rmeObject->GetRobot()->UndoLastMovement();
}

void RobotCollisionResolver::Initilize() {
	if(robotCollisionResolver == NULL)
		robotCollisionResolver = new RobotCollisionResolver();
}