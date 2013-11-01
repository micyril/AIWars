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
		
		float calculateDistance(float x0, float y0, float x1, float y1) {
			float deltaX = x1 - x0;
			float deltaY = y1 - y0;
			return sqrt(deltaX * deltaX + deltaY * deltaY);
		}

		TEST_METHOD(TestExecuteMove)
		{
			float x0, y0;
			x0 = y0 = 100.0F;
			RobotFrame *robotFrame = new RobotFrame(50, 50, x0, y0);
			float movingSpeed = 50.5F;
			RobotComponent *runningGear = new RunningGear(movingSpeed, 0);
			std::list<RobotComponent*> robotComponents;
			robotComponents.push_back(runningGear);
			Robot *robot = new Robot(robotFrame, robotComponents);

			robot->Execute("MOV", "50.5");
			robot->Update(0.5F);
			robot->Update(0.5F);
			float actualDistance = calculateDistance(x0, y0, robot->frame->x, robot->frame->y);
			Assert::IsTrue(abs(50.5 - actualDistance) < 0.001F);
			robot->Update(0.5F);
			actualDistance = calculateDistance(x0, y0, robot->frame->x, robot->frame->y);
			Assert::IsTrue(abs(50.5 - actualDistance) < 0.001F);
		}

		TEST_METHOD(TestExecuteRotate)
		{
			RobotFrame *robotFrame = new RobotFrame(50, 50, 100, 100);
			float rotation0 = robotFrame->rotation;
			float rotationSpeed = 1.0F;
			RobotComponent *runningGear = new RunningGear(0, rotationSpeed);
			std::list<RobotComponent*> robotComponents;
			robotComponents.push_back(runningGear);
			Robot *robot = new Robot(robotFrame, robotComponents);

			robot->Execute("ROT", "2.0");
			robot->Update(1.0F);
			robot->Update(1.0F);
			float actualDeltaRotation = robot->frame->rotation - rotation0;
			Assert::AreEqual(2.0F, actualDeltaRotation);
			robot->Update(1.0F);
			actualDeltaRotation = robot->frame->rotation - rotation0;
			Assert::AreEqual(2.0F, actualDeltaRotation);
		}
	};
}