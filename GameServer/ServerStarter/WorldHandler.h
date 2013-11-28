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
#include <vector>
struct clientInfo{
	int winner;
	Client* c;
	Robot* r;
	char* isWorking;
};
struct worldInfo{
	int max_clients;
	World* world;
	std::vector<clientInfo*>* clinfo;
	std::vector<char>* active_clients;
};



class WorldHandler
{
	/*static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL pool;*/
	int max_clients;
	Robot* makeRobot(int width, int hieght, float x, float y,  World* world);
public:
	WorldHandler(int max_clients_in_game);
	void startGame(Client* cl1, Client* cl2 );
	~WorldHandler(void);
};

