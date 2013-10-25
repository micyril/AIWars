#include "WorldHandler.h"
#define MAX_CONNECTIONS (100)

WorldHandler::WorldHandler(void)
{
	InitializeThreadpoolEnvironment(&pool_env);
	pool = CreateThreadpool(0);		
	SetThreadpoolThreadMinimum(pool, MAX_CONNECTIONS);
	SetThreadpoolThreadMaximum(pool, MAX_CONNECTIONS);
	SetThreadpoolCallbackPool(&pool_env, pool);

	worldId = 0;
}
void WorldHandler::start(Client* cl1, Client* cl2){
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
	World* world = new World(500, 500, worldObjects, mapElements);
	this->worldls[worldId++] = world;
	cl1->worldId = worldId;
	cl2->worldId = worldId;
}
WorldHandler::~WorldHandler(void)
{
	//TODO delete wordls
}
