#include "gun.h"
#include "../../exceptions.h"

GunBarrel::GunBarrel(int length, int width) : RobotMapElement("GunBarrel", length, width, Point(0.0F, 0.0F)) {}

Gun::Gun(World *world, int length, int width, float rateOfFire, float bulletsSpeed, int bulletsDamageInfluence) : 
	world(world), barrel(new GunBarrel(length, width)), bulletFactory(barrel, world, bulletsSpeed, bulletsDamageInfluence), nextShotWaitingTime(0.0F) {  //todo: do it more beautiful
		mapElements.push_back(barrel);
		reloadingTime = 1.0F / rateOfFire;
		supportedCommands.push_back("FR");
}

Gun::~Gun() {
	delete barrel;
}

void Gun::SetRobot(Robot *robot) {
	RobotComponent::SetRobot(robot);
	barrel->SetRobot(robot);

	barrel->vertice.x = robot->frame->rotationCenter.x;
	barrel->vertice.y = robot->frame->rotationCenter.y - 0.5F * barrel->height;
	barrel->rotationCenter = robot->frame->rotationCenter;
}

std::string Gun::Execute(const std::string &command, const std::string &arg) {
	//TODO: think out what should be done if we are reloading now
	//TODO: think out what should be returned if we are reloading now
	if (command == "FR") {
		if(nextShotWaitingTime == 0 && barrel->GetHealth() > 0)
			nextShotWaitingTime = reloadingTime;
		return std::string("");  //TODO: use constant
	}
	throw NotSupportedCommandException(command);
}

void Gun::Update(float delta) {
	if(nextShotWaitingTime == reloadingTime) {
		Bullet *bullet = bulletFactory.Create();
		world->Add(bullet);
	}
	if(nextShotWaitingTime > 0)
		nextShotWaitingTime = std::max<float>(nextShotWaitingTime - delta, 0.0F);
}