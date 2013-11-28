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
			RobotMapElement element1(40, 40, 200, 200);
			RobotMapElement element2(40, 10, 162, 215);
 
			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));
			Assert::IsTrue(CollisionChecker::Check(&element2, &element1));
		}

		TEST_METHOD(TestCheckWithChangingRotationCenter)
		{
			RobotMapElement element1(40, 40, 200, 200);
			RobotMapElement element2(40, 10, 162, 215);
			element2.rotationCenterX = 162;
			element2.rotationCenterY = 220;
 
			Assert::IsTrue(CollisionChecker::Check(&element1, &element2));

			RobotMapElement element3(40, 10, 120, 215);
			element3.rotationCenterX = 120;
			element3.rotationCenterY = 220;
			RobotMapElement element4(8, 10, 160.001F, 215);
			element4.rotationCenterX = 120;
			element4.rotationCenterY = 220;
 
			Assert::IsFalse(CollisionChecker::Check(&element3, &element4));
		}

	};
}