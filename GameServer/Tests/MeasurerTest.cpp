#include "CppUnitTest.h"
#include "../GameServer/world/mapelement.h"
#include "../GameServer/world/collisions/collisionchecker.h"
#include "../GameServer/world/robot/robot.h"
#include "../GameServer/world/robot/components/gun/gun.h"
#include "../GameServer/world/robot/components/visualscanner/measurer.h"
#define M_PI       3.14159265358979323846
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(MeasurerTest)
	{
	public:
		TEST_METHOD(TestMeasureSimpleCheck)
		{
			Point p(100.0f, 100.0f);
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(100.0f, 100.0f, Point(60.0f, 150.0f));
			float dist;
			float angle;
			Measurer::FindDistanceAndDirectionToRectangle(&me, p,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 50.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureRotationCheck)
		{
			Point p(100.0f, 100.0f);
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(100.0f, 100.0f, Point(60.0f, 150.0f), M_PI/2.0f);
			float dist;
			float angle;
			Measurer::FindDistanceAndDirectionToRectangle(&me, p,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 50.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureRotationNonSquereCheck)
		{
			Point p(100.0f, 100.0f);
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(120.0f, 100.0f, Point(60.0f, 150.0f), M_PI/2.0f);
			float dist;
			float angle;
			Measurer::FindDistanceAndDirectionToRectangle(&me, p,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 40.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureonEdgeLeftCheck)
		{
			Point p(100.0f, 100.0f);
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 100.0f;
			Rectangle me(90.0f, 90.0f, Point(0.0f, 100.0f));
			float dist;
			float angle;
			Measurer::FindDistanceAndDirectionToRectangle(&me, p,begin, end, max_distance,dist, angle);
			float test_dist = fabs(10.0f/cos(2.0f*M_PI/3.0f));
			Assert::IsTrue(fabs(dist - test_dist) < 1e-4);
			Assert::IsTrue(fabs(angle -  end) < 1e-5);
		}
		TEST_METHOD(TestMeasureonEdgeRightCheck)
		{
			Point p(100.0f, 100.0f);
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 100.0f;
			Rectangle me(90.0f, 90.0f, Point(110.0f, 100.0f));
			float dist;
			float angle;
			Measurer::FindDistanceAndDirectionToRectangle(&me, p,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 20.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/3.0f) < 1e-5);
		}
		TEST_METHOD(TestFindDistanceAndDirectionToAcrossRectangle)
		{
			Point p(100.0f, 100.0f);
			float sectorBegin = -M_PI/6.0f;
			float sectorEnd = M_PI/6.0f;
			float max_distance = 120.0f;

			Rectangle rectangle(50, 50, Point(175, 75));
			float distance;
			float direction;

			Measurer::FindDistanceAndDirectionToRectangle(&rectangle, p, sectorBegin, sectorEnd, max_distance, distance, direction);
			Assert::IsTrue(fabs(distance - 75.0f) < 1e-5);
			Assert::IsTrue(fabs(direction) < 1e-5);
		}
	};
}