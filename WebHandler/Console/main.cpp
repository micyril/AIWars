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
	w->Update(0);
	while (1) {
		c->notifyUpdate(w->getElements());
		Sleep(20);
	}
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
	Robot* r1 = makeRobot(40, 40, 10.0f, 10.0f);
	Robot* r2 = makeRobot(40, 40, 10.0f, 450.0f);
	Robot* r3 = makeRobot(40, 40, 450.0f, 10.0f);
	Robot* r4 = makeRobot(40, 40, 450.0f, 450.0f);

	w = new World(500, 500);
	w->Add(r1);
	w->Add(r2);
	w->Add(r3);
	w->Add(r4);

	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
