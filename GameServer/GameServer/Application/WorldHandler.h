#pragma once
#include <Windows.h>
#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "world\mapelement.h"
#include "world\world.h"
#include "world\robot\components\runninggear.h"
#include "world\robot\robot.h"
#include <map>
class WorldHandler
{
	static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL pool;
	static std::map<int, World*> worldls;
	int worldId;
public:
	WorldHandler(void);
	void start(Client* cl1, Client* cl2 );
	~WorldHandler(void);
};

