#include <iostream>
#include "world\mapelement.h"
#include "world\world.h"
#include "world\robot\components\runninggear.h"
#include "world\robot\robot.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"

void OnConnect(Client *c) {
	RobotFrame *robotFrame = new RobotFrame(50, 100, 200, 200);
	float speed = 10;
	RobotComponent *runningGear = new RunningGear(speed);
	std::map<std::string, RobotComponent*> commandToRobotComponent;
	commandToRobotComponent["move"] = runningGear;
	Robot *robot = new Robot(robotFrame, commandToRobotComponent);
	std::list<WorldObject*> worldObjects;
	std::list<MapElement*> mapElements;
	worldObjects.push_back(robot);
	mapElements.push_back(robotFrame);
	World world(500, 500, worldObjects, mapElements);

	c->sendSelfInfo();
	Sleep(2000);
	c->sendEnemyInfo(20);
	Sleep(1000);
	c->sendGameInfo(&world);
	c->notifyStart();
	c->notifyUpdate(world.getElements());
	Sleep(2000);

	float arg = 100;
	robot->Execute("move", &arg);
	int sleepPeriod = 50;
	while(true) {
		Sleep(sleepPeriod);
		world.Update(sleepPeriod / 1000.0);
		c->notifyUpdate(world.getElements());
	}
	c->notifyUpdate(world.getElements());
	Sleep(2000);
	c->notifyFinish(c->id);
}

int test()
{
	//World w(500,500);
	//std::cout << w.Serialize();
	return 0;
}
int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE); // join к потоку

	return test();
}