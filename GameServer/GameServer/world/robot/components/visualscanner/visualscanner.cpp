#define _USE_MATH_DEFINES
#include <math.h>
#include <set>
#include <sstream>
#include "visualscanner.h"
#include "../../exceptions.h"
#include "../../robot.h"
#include "../../../collisions/collisionchecker.h"
#include "measurer.h"

VisualScanner::VisualScanner(World *world, float viewAngle, float viewDistance) : 
	world(world), viewAngle(viewAngle), viewDistance(viewDistance) {
		supportedCommands.push_back("SC");

		scanRectangleHeight = viewDistance * cos(M_PI_2 - viewAngle / 2.0F);
}

void VisualScanner::SetRobot(Robot *robot) {
	RobotComponent::SetRobot(robot);
}

std::string VisualScanner::Execute(const std::string &command, const std::string &arg) {
	if (command == "SC") {
		float pointOfViewX = robot->frame->rotationCenterX;
		float pointOfViewY = robot->frame->rotationCenterY;
		float sectorBegin = robot->frame->rotation - viewAngle / 2.0F;
		float sectorEnd = robot->frame->rotation + viewAngle / 2.0F;

		Rectangle scanRectangle1(viewDistance, scanRectangleHeight, pointOfViewX, pointOfViewY, 
			pointOfViewX, pointOfViewY, sectorEnd);
		Rectangle scanRectangle2(viewDistance, scanRectangleHeight, pointOfViewX, pointOfViewY - scanRectangleHeight, 
			pointOfViewX, pointOfViewY, sectorBegin);

		std::stringstream serializationStream;
		serializationStream << "RET";
		std::string space = " ";
		auto allMapElements = world->GetMapElements();
		float distanceToMapElement, directionToMapElement;
		for(auto mapElemIt = allMapElements.begin(); mapElemIt != allMapElements.end(); mapElemIt++) {
			if (CollisionChecker::Check(&scanRectangle1, *mapElemIt) && CollisionChecker::Check(&scanRectangle2, *mapElemIt) && 
				((*mapElemIt)->GetType() != RobotMapElement::GetClassType() || ((RobotMapElement*)*mapElemIt)->GetRobot() != robot)) {
					Measurer::FindDistanceAndDirectionToRectangle(*mapElemIt, pointOfViewX, pointOfViewY, sectorBegin, sectorEnd, viewDistance, 
						distanceToMapElement, directionToMapElement);
					serializationStream << space << (*mapElemIt)->GetType() << space << distanceToMapElement << space << directionToMapElement;
			}
		}

		return serializationStream.str();
	}
	throw NotSupportedCommandException(command);
}

void VisualScanner::Update(float delta) {

}