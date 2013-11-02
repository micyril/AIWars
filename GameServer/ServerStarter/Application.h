#pragma once
#include <list>
#include "..\GameServer\world\mapelement.h"
#include "..\GameServer\world\world.h"
#include "..\GameServer\world\robot\components\runninggear.h"
#include "..\GameServer\world\robot\robot.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"
#include "WorldHandler.h"
#include <map>
#include <set>
#include <sstream>
class Application
{
	static std::list<Client*> waitingClients;
	static std::mutex m;
	static std::map<int, Client*> clients;
	static std::set<int> isClientConnect;
	static std::map<int, int> clientsToRun;
	static void listenOnPort(int port);
public:
	Application(void);
	static void onConnect(Client *c);
	
	static void Run();
	~Application(void);
};

