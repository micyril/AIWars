#include <cmath>
#include "CppUnitTest.h"
#include "../GameServer/world/robot/components/runninggear.h"
#include "../GameServer/world/robot/robot.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(RunningGearTest)
	{
	public:
		
		
		TEST_METHOD(TestExecuteMove)
		{
			Point p(100.0f, 100.0f);
			RobotFrame *robotFrame = new RobotFrame(50, 50, p);
			float movingSpeed = 50.5F;
			RobotComponent *runningGear = new RunningGear(movingSpeed, 0);
			std::list<RobotComponent*> robotComponents;
			robotComponents.push_back(runningGear);
			Robot *robot = new Robot(robotFrame, robotComponents);

			robot->Execute("MOV", "50.5");
			robot->Update(0.5F);
			robot->Update(0.5F);
			float actualDistance = distanceToPoint(p, robot->frame->vertice);
			Assert::IsTrue(abs(50.5 - actualDistance) < 0.001F);
			robot->Update(0.5F);
			actualDistance = distanceToPoint(p, robot->frame->vertice);
			Assert::IsTrue(abs(50.5 - actualDistance) < 0.001F);
		}

		TEST_METHOD(TestExecuteRotate)
		{
			RobotFrame *robotFrame = new RobotFrame(50, 50, Point(100, 100));
			float rotation0 = robotFrame->getAngle();
			float rotationSpeed = 1.0F;
			RobotComponent *runningGear = new RunningGear(0, rotationSpeed);
			std::list<RobotComponent*> robotComponents;
			robotComponents.push_back(runningGear);
			Robot *robot = new Robot(robotFrame, robotComponents);

			robot->Execute("ROT", "2.0");
			robot->Update(1.0F);
			robot->Update(1.0F);
			float actualDeltaRotation = robot->frame->getAngle() - rotation0;
			Assert::AreEqual(2.0F, actualDeltaRotation);
			robot->Update(1.0F);
			actualDeltaRotation = robot->frame->getAngle() - rotation0;
			Assert::AreEqual(2.0F, actualDeltaRotation);
		}
	};
}