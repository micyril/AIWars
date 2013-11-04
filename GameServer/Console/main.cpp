#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "..\..\GameServer\GameServer\world\world.h"
#include "..\..\GameServer\GameServer\world\robot\components\runninggear.h"
#include "..\..\GameServer\GameServer\world\robot\robot.h"

Robot* makeRobot(int width, int hieght, float x, float y) {
	RobotFrame *robotFrame = new RobotFrame(width, hieght, x, y);
	float movingSpeed = 20;
	float rotationSpeed = 0.5;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	return new Robot(robotFrame, robotComponents);
}

void OnConnect(Client *c) {
	Robot* r1 = makeRobot(40, 40, 210.0f, 100.0f);
	Robot* r2 = makeRobot(40, 40, 200.0f, 200.0f);

	std::list<WorldObject*> worldObjects;
	worldObjects.push_back(r1);
	worldObjects.push_back(r2);

	World world = World(500, 500, worldObjects);

	c->sendSelfInfo();
	c->sendEnemyInfo(20);
	c->sendGameInfo(&world);
	c->notifyStart();	

	r1->Execute("MOV", "120.0");
	r1->Execute("ROT", "-1.57");
	int sleepPeriod = 20;
	while(true) {
		Sleep(sleepPeriod);
		world.Update(sleepPeriod / 1000.0);
		c->notifyUpdate(world.getElements());
	}
}

int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
