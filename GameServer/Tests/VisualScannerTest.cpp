#include "CppUnitTest.h"
#include <sstream>
#include "..\..\GameServer\GameServer\world\world.h"
#include "..\..\GameServer\GameServer\world\robot\components\runninggear.h"
#include "..\..\GameServer\GameServer\world\robot\robot.h"
#include "..\..\GameServer\GameServer\world\robot\components\gun\gun.h"
#include "..\..\GameServer\GameServer\world\robot\components\visualscanner\visualscanner.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(VisualScannerTest)
	{
	public:
		static int FindCountOfElements(std::string &scanResult) {
			std::string buf; // Have a buffer string
			std::stringstream ss(scanResult); // Insert the string into a stream

			int i = 0;
			while (ss >> buf)
				i++;
			return (i - 1) / 3;
		};

		static Robot* createRobot(int width, int height, float x, float y) {
			RobotFrame *robotFrame = new RobotFrame(width, height, x, y);
			std::list<RobotComponent*> robotComponents;
			return new Robot(robotFrame, robotComponents);
		}

		static Robot* createRobotWithGunAndScanner(World *world, VisualScanner *visualScanner, int width, int height, float x, float y) {
			RobotFrame *robotFrame = new RobotFrame(width, height, x, y);
			Gun *gun = new Gun(world, 50, 50 / 4);
			std::list<RobotComponent*> robotComponents;
			robotComponents.push_back(gun);
			robotComponents.push_back(visualScanner);
			return new Robot(robotFrame, robotComponents);
		}
		
		TEST_METHOD(TestScanWithNoMapElements)
		{
			World world(500, 500);

			float viewAngle = 1.0F;
			float viewDistance = 120.0F;
			VisualScanner *visualScanner = new VisualScanner(&world, viewAngle, viewDistance);

			Robot *robot = createRobotWithGunAndScanner(&world, visualScanner, 50, 50, 75, 75);

			world.Add(robot);
			world.Update(0.0F);
			
			std::string actual = visualScanner->Execute("SC", "");
			Assert::AreEqual(0, FindCountOfElements(actual));
		}

		TEST_METHOD(TestScanWithMapElements)
		{
			World world(500, 500);

			float viewAngle = 1.57F;
			float viewDistance = 120.0F;
			VisualScanner *visualScanner = new VisualScanner(&world, viewAngle, viewDistance);

			Robot *robot = createRobotWithGunAndScanner(&world, visualScanner, 50, 50, 75, 75);
			Robot *robot2 = createRobot(50, 50, 175, 75);
			Robot *robot3 = createRobot(50, 50, 90, 130);

			world.Add(robot);
			world.Add(robot2);
			world.Update(0.0F);
			
			std::string actual = visualScanner->Execute("SC", "");
			Assert::AreEqual(1, FindCountOfElements(actual));

			world.Add(robot3);
			world.Update(0.0F);

			actual = visualScanner->Execute("SC", "");
			Assert::AreEqual(2, FindCountOfElements(actual));
		}

		TEST_METHOD(TestScanWithMapElements2)
		{
			World world(500, 500);

			float viewAngle = 1.57F;
			float viewDistance = 120.0F;
			VisualScanner *visualScanner = new VisualScanner(&world, viewAngle, viewDistance);

			Robot *robot = createRobotWithGunAndScanner(&world, visualScanner, 50, 50, 75, 75);
			Robot *robot2 = createRobot(50, 50, 80, 130);

			world.Add(robot);
			world.Add(robot2);
			world.Update(0.0F);
			
			std::string actual = visualScanner->Execute("SC", "");
			Assert::AreEqual(0, FindCountOfElements(actual));
		}

		TEST_METHOD(TestScanWithMapElements3)
		{
			World world(500, 500);

			float viewAngle = 1.57F;
			float viewDistance = 120.0F;
			VisualScanner *visualScanner = new VisualScanner(&world, viewAngle, viewDistance);

			Robot *robot = createRobotWithGunAndScanner(&world, visualScanner, 50, 50, 75, 75);
			Robot *robot2 = createRobot(50, 50, 90, 130);

			world.Add(robot);
			world.Add(robot2);
			world.Update(0.0F);
			
			std::string actual = visualScanner->Execute("SC", "");
			Assert::AreEqual(1, FindCountOfElements(actual));
		}
	};
}