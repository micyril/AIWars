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
		c->notifyUpdate((std::list<Serializable*>*)w->GetMapElements());
		Sleep(200);
	}
}

Robot* makeRobot(int width, int height, Point p, World* world){
	RobotFrame *robotFrame = new RobotFrame(width, height, p);
	float movingSpeed = 20;
	float rotationSpeed = 0.5;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	RobotComponent *gun = new Gun(world, width, width / 4);
	VisualScanner *visualScanner = new VisualScanner(world);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	robotComponents.push_back(gun);
	robotComponents.push_back(visualScanner);
	return new Robot(robotFrame, robotComponents);
}

int main() {
	w = new World(500, 500);
	w->Add(makeRobot(40, 40, Point(10, 10), w));
	w->Add(makeRobot(40, 40, Point(10, 450), w));
	w->Add(makeRobot(40, 40, Point(450, 10), w));
	w->Add(makeRobot(40, 40, Point(450, 450), w));

	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
