#include "CppUnitTest.h"
#include "../GameServer/world/collisions/collisionresolvermaster.h"
#include "../GameServer/world/robot/robot.h"
#include "../GameServer/exceptions.h"
#include "../GameServer/world/collisions/exceptions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CollisionResolverMasterTest)
	{
	public:
		class NewMapElement : public MapElement {
		public:
			NewMapElement() : MapElement(std::string("NewMapElement"), std::string("NewMapElement"), 0, 0, Point(0.0f, 0.0f)) {}
		};

		TEST_METHOD(TestResolve)
		{
			RobotFrame robotFrame1(10, 10);
			RobotFrame robotFrame2(20, 20);

			try {
				CollisionResolverMaster::Resolve(&robotFrame1, &robotFrame2);
			}
			catch (NotImplementedException) {}
		}

		TEST_METHOD(TestResolveWithCollisionResolverNotFoundException)
		{
			NewMapElement newMapElement1;
			NewMapElement newMapElement2;

			try {
				CollisionResolverMaster::Resolve(&newMapElement1, &newMapElement2);
				throw 1;
			}
			catch (CollisionResolverNotFoundException) {}
		}
	};
}