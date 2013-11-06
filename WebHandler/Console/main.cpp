#include "..\WebHandler\WebHandler.h"
#include "..\..\GameServer\ServerStarter\Application.h"

World *w;

void OnConnect(Client *c) {
	c->sendSelfInfo();
	Sleep(500);
	c->sendEnemyInfo(20);
	Sleep(500);
	c->sendGameInfo(w);
	Sleep(500);
	c->notifyStart();
	c->notifyUpdate(w->getElements());
}

Robot* makeRobot(int width, int hieght, float x, float y) {
	RobotFrame *robotFrame = new RobotFrame(width, hieght, x, y);
	float movingSpeed = 40;
	float rotationSpeed = 0.005;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	return new Robot(robotFrame, robotComponents);
}

int main() {
	Robot* r1 = makeRobot(40,40, 100.0f, 100.0f);
	Robot* r2 = makeRobot(40,40, 200.0f, 200.0f);

	std::list<WorldObject*> worldObjects;
	worldObjects.push_back(r1);
	worldObjects.push_back(r2);

	w = new World(500, 500, worldObjects);

	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
