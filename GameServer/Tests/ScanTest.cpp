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
	TEST_CLASS(CollisionCheckerTest)
	{
	public:

		TEST_METHOD(TestMeasureSimpleCheck)
		{
			float x = 100.0f;
			float y = 100.0f;
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(100.0f, 100.0f, 60.0f, 150.0f);
			Measurer m;
			float dist;
			float angle;
			m.FindDistanceAndDirectionToRectangle(&me, x,y,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 50.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureRotationCheck)
		{
			float x = 100.0f;
			float y = 100.0f;
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(100.0f, 100.0f, 60.0f, 150.0f, M_PI/2.0f);
			Measurer m;
			float dist;
			float angle;
			m.FindDistanceAndDirectionToRectangle(&me, x,y,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 50.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureRotationNonSquereCheck)
		{
			float x = 100.0f;
			float y = 100.0f;
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 60.0f;
			Rectangle me(120.0f, 100.0f, 60.0f, 150.0f, M_PI/2.0f);
			Measurer m;
			float dist;
			float angle;
			m.FindDistanceAndDirectionToRectangle(&me, x,y,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 40.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/2.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureonEdgeLeftCheck)
		{
			float x = 100.0f;
			float y = 100.0f;
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 100.0f;
			Rectangle me(90.0f, 90.0f, 0.0f, 100.0f);
			Measurer m;
			float dist;
			float angle;
			m.FindDistanceAndDirectionToRectangle(&me, x,y,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 20.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  M_PI/3.0f) < 1e-5);
		}
		TEST_METHOD(TestMeasureonEdgeRightCheck)
		{
			float x = 100.0f;
			float y = 100.0f;
			float begin = M_PI/3.0f;
			float end = 2.0f*M_PI/3.0f;
			float max_distance = 100.0f;
			Rectangle me(90.0f, 90.0f, 110.0f, 100.0f);
			Measurer m;
			float dist;
			float angle;
			m.FindDistanceAndDirectionToRectangle(&me, x,y,begin, end, max_distance,dist, angle);
			Assert::IsTrue(fabs(dist - 20.0f) < 1e-5);
			Assert::IsTrue(fabs(angle -  2.0f*M_PI/3.0f) < 1e-5);
		}
	};
}