#include <iostream>
#include "..\GameServer\world\mapelement.h"
#include "..\GameServer\world\world.h"
#include "..\GameServer\world\robot\components\runninggear.h"
#include "..\GameServer\world\robot\robot.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "..\GameServer\ClientHandler\ClientHandler.h"
void OnConnect(Client *c) {
	RobotFrame *robotFrame = new RobotFrame(50, 50, 200, 200);
	float movingSpeed = 40;
	float rotationSpeed = 0.005;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	Robot *robot = new Robot(robotFrame, robotComponents);
	std::list<WorldObject*> worldObjects;
	worldObjects.push_back(robot);
	World world(500, 500, worldObjects);
	//ClientHandler cHandler(robot, "2560");
	c->sendSelfInfo();
	Sleep(2000);
	c->sendEnemyInfo(20);
	Sleep(1000);
	c->sendGameInfo(&world);
	c->notifyStart();
	c->notifyUpdate(world.getElements());
	Sleep(2000);

	float arg = 100;
	float rotArg = 1.54F;
	robot->Execute("move", &arg);
	//robot->Execute("rotate", &rotArg);
	int sleepPeriod = 20;
	while(true) {
		Sleep(sleepPeriod);
		//cHandler.Execute();//TODO move it to other thread
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