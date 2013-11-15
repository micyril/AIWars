#pragma once
#include <Windows.h>
#include <sstream>
#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "..\GameServer\world\mapelement.h"
#include "..\GameServer\world\world.h"
#include "..\GameServer\world\robot\components\runninggear.h"
#include "..\GameServer\world\robot\components\gun\gun.h"
#include "..\GameServer\world\robot\robot.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "..\GameServer\exceptions.h"
#include <map>
struct clientInfo{
	Client* c;
	Robot* r;
};
struct worldInfo{
	World* world;
	clientInfo* c1info;
	clientInfo* c2info;
};



class WorldHandler
{
	/*static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL pool;*/
	std::map<int, World*> worlds;
	int worldId;

	Robot* makeRobot(int width, int hieght, float x, float y,  World* world);
public:
	WorldHandler(void);
	void startGame(Client* cl1, Client* cl2 );
	~WorldHandler(void);
};

