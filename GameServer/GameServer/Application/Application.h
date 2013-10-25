#pragma once
#include <list>
#include "..\..\WebHandler\WebHandler\Client.h"
#include "exceptions.h"
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

