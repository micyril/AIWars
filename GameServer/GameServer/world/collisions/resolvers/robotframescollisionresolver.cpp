#include "robotframescollisionresolver.h"
#include "../collisionresolvermaster.h"
#include "../../../exceptions.h"
#include "../../robot/robot.h"

RobotFramesCollisionResolver* RobotFramesCollisionResolver::robotFramesCollisionResolver = NULL;

RobotFramesCollisionResolver::RobotFramesCollisionResolver() {
	CollisionResolverMaster::AddNewCollisionResolver(RobotFrame::GetClassType(), RobotFrame::GetClassType(), this);
}

void RobotFramesCollisionResolver::Resolve(MapElement *mapElement1, MapElement *mapElement2) {
	RobotFrame *frame1 = (RobotFrame *)mapElement1;
	RobotFrame *frame2 = (RobotFrame *)mapElement2;

	//todo:: implemention
	throw NotImplementedException();
}

void RobotFramesCollisionResolver::Initilize() {
	if(robotFramesCollisionResolver == NULL)
		robotFramesCollisionResolver = new RobotFramesCollisionResolver();
}