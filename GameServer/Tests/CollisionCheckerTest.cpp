#include "CppUnitTest.h"
#include "../GameServer/world/mapelement.h"
#include "../GameServer/world/collisions/collisionchecker.h"
#include "../GameServer/world/robot/robot.h"
#include "../GameServer/world/robot/components/gun/gun.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CollisionCheckerTest)
	{
	public:
		TEST_METHOD(TestCheck)
		{
			RobotFrame element1(40, 40, Point(200, 200));
			RobotFrame element2(40, 10, Point(162, 215));
 
			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));
			Assert::IsTrue(CollisionChecker::Check(&element2, &element1));
		}

		TEST_METHOD(TestCheckWithChangingRotationCenter)
		{
			RobotFrame element1(40, 40, Point(200, 200));
			RobotFrame element2(40, 10, Point(162, 215));
			element2.rotationCenter.x = 162;
			element2.rotationCenter.y = 220;
 
			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));

			RobotFrame element3(40, 10, Point(120, 215));
			element3.rotationCenter.x = 120;
			element3.rotationCenter.y = 220;
			RobotFrame element4(8, 10, Point(160.001F, 215));
			element4.rotationCenter.x = 120;
			element4.rotationCenter.y = 220;
 
			Assert::IsFalse(CollisionChecker::Check(&element3, &element4));
		}

		TEST_METHOD(TestCheckRotatedElements)
		{
			RobotFrame element1(40, 40, Point(200, 200));
			RobotFrame element2(40, 10, Point(196.488998F, 171.402786F));
			element2.rotationCenter.x = 196.488998F;
			element2.rotationCenter.y = 176.402786F;
			element2.setAngle(-0.75F);

			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));
		}

		TEST_METHOD(TestCheckCollisionBelong)
		{
			RobotFrame element1(100, 100, Point(200, 200));
			RobotFrame element2(40, 40, Point(250, 250));


			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));
		}
	};
}