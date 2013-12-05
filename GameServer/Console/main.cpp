#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "..\..\GameServer\GameServer\world\world.h"
#include "..\..\GameServer\GameServer\world\robot\components\runninggear.h"
#include "..\..\GameServer\GameServer\world\robot\robot.h"
#include "..\..\GameServer\GameServer\world\robot\components\gun\gun.h"

Robot* makeRobot(int width, int height, float x, float y, World *world) {
	RobotFrame *robotFrame = new RobotFrame(width, height, x, y);
	float movingSpeed = 20;
	float rotationSpeed = 0.5;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	RobotComponent *gun = new Gun(world, width, width / 4);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	robotComponents.push_back(gun);
	return new Robot(robotFrame, robotComponents);
}

void doLoobBody(World &world, Client* const c, int sleepPeriod) {
	Sleep(sleepPeriod);
	world.Update(sleepPeriod / 1000.0F);
	c->notifyUpdate((std::list<Serializable*>*)world.GetMapElements());
}

void OnConnect(Client *c) {
	World world = World(500, 500);

	Robot* r1 = makeRobot(40, 40, 100.0f, 200.0f, &world);
	Robot* r2 = makeRobot(40, 40, 200.0f, 200.0f, &world);

	world.Add(r1);
	world.Add(r2);

	c->sendSelfInfo();
	c->sendEnemyInfo(20);
	c->sendGameInfo(&world);
	c->notifyStart();
	int sleepPeriod = 20;

	//r1->Execute("MOV", "120.0");
	r1->Execute("FR", "");
	//r1->Execute("ROT", "-0.75");
	//r2->Execute("FR", "");
	
	for(int i = 0; i < 26; i++)
		doLoobBody(world, c, sleepPeriod);
	//r1->Execute("FR", "");
	//r2->Execute("FR", "");
	while(true)
		doLoobBody(world, c, sleepPeriod);
}

int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
