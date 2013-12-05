#include "WorldHandler.h"
#include <iostream>
#include "time.h"
#include "const.h"
#define MAX_CONNECTIONS (100)

WorldHandler::WorldHandler(int max_clients)
{
	/*InitializeThreadpoolEnvironment(&pool_env);
	pool = CreateThreadpool(0);		
	SetThreadpoolThreadMinimum(pool, MAX_CONNECTIONS);
	SetThreadpoolThreadMaximum(pool, MAX_CONNECTIONS);
	SetThreadpoolCallbackPool(&pool_env, pool);*/
	this->max_clients = 2;
}
Robot* WorldHandler::makeRobot(int width, int height, float x, float y, World* world){
	RobotFrame *robotFrame = new RobotFrame(width, height, x, y);
	float movingSpeed = 20;
	float rotationSpeed = 0.5;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	RobotComponent *gun = new Gun(world, width, width / 4);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
	robotComponents.push_back(gun);
	return new Robot(robotFrame, robotComponents);
}
void parceCommand(char* input, int size, std::string &command, std::string &arg){
	std::string instr(input, input + size);
	std::stringstream ss(instr);
	std::string index("");
	command = "";
	std::string argsstr("");
	std::getline(ss, index, ' ');//skip index
	std::getline(ss, command, ' ');
	std::getline(ss, arg, ' ');
}
#include "../GameServer/world/robot/exceptions.h" //TODO
DWORD WINAPI clientThread(LPVOID lpParam){

	clientInfo* info = (clientInfo*)lpParam;

	int iResult;
	int iSendResult;
    char recvbuf[1024];
    int recvbuflen = 1024;
	std::string command("");
	std::string arg;
	
	send(info->c->commandSocket, ack.c_str(), ack.length(), 0);
	try{
		 while (1) {
			iResult = recv(info->c->commandSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				if (std::string(recvbuf).find(eog) != -1){
					cerr << "End of game client: " << info->c->id << std::endl;
					iSendResult = send( info->c->commandSocket, eog.c_str(), eog.length(), 0 );
					closesocket(info->c->commandSocket); 
					//delete info->r;
					//delete info->c;
					*(info->isWorking) = 0;
					return 0;
				}
				parceCommand(recvbuf, iResult, command, arg);
			
				try{
					std::string answer = info->r->Execute(command, arg); 
					iSendResult = send( info->c->commandSocket, ack.c_str(), ack.length(), 0 );
				}
				catch(NotSupportedCommandException &ex){
					cerr << "fail command" << endl;

				}
				catch(...){
					std::cerr << "FAIL" << std::endl;
				}
				if (iSendResult == SOCKET_ERROR) {
					throw SocketConnectionException(); 
				}
			}
			else{
				cerr << "Client was crashed: " << info->c->id << std::endl;
				iSendResult = send( info->c->commandSocket, eog.c_str(), eog.length(), 0 );
				closesocket(info->c->commandSocket); 
				//delete info->r;
				//delete info->c;
				*(info->isWorking) = -1;
				return 1;
			}
		}
	}
	catch(SocketConnectionException &ex){
		std::cerr << "fail socket" << std::endl;
		closesocket(info->c->commandSocket); 
		//delete info->r;
		//delete info->c;
		*(info->isWorking) = -1;
		return 1;
	}
	catch(...){
		std::cerr << "FAIL" << std::endl;
		closesocket(info->c->commandSocket); 
		//delete info->r;
		//delete info->c;
		*(info->isWorking) = -1;
		return 1;
	}
}

DWORD WINAPI worldThread( LPVOID lpParam ){
	worldInfo* info = (worldInfo*)lpParam;
	int sleepPeriod = 20;

	for(int i = 0;i<info->max_clients;i++){
		(*info->clinfo)[i]->c->sendSelfInfo();
		//(*clients)[i]->c->sendEnemyInfo(info->c2info->c->id);
		(*info->clinfo)[i]->c->sendGameInfo(info->world);
		(*info->clinfo)[i]->c->notifyStart();
		CreateThread(NULL, NULL, clientThread, (*info->clinfo)[i], NULL, NULL);
	}
	Sleep(sleepPeriod);
	clock_t start, end;
	bool game_alive = true;
	int winner = 0;
	while(game_alive) {
		game_alive = false;
		start = clock();
		Sleep(20);//dont work without
		info->world->Update(sleepPeriod / 1000.0f);
		//(*clients)[i]->winner = info->world->winner;
		for(int i = 0;i<info->max_clients;i++){
			(*info->clinfo)[i]->c->notifyUpdate((std::list<Serializable*>*)(info->world->GetMapElements()));
			switch((*info->active_clients)[i]){
				case 0:
					break;
				case 1:
					game_alive = true;
					break;
				case -1:
					winner = (*info->clinfo)[i]->c->id;//enemy id here
					break;
			}
		}
		end = clock();
		sleepPeriod = end - start;
	}
	std::cerr << "Game was ended: winner is " << winner << std::endl;
	for(int i = 0;i<info->max_clients;i++){
		(*info->clinfo)[i]->c->notifyFinish(winner);
		delete (*info->clinfo)[i];
	}
	delete info->active_clients;
	delete info->clinfo;
	delete info->world;
	delete info;
	return 0;
}


void WorldHandler::startGame(Client* cl1, Client* cl2){
	
	World* world = new World(500, 500);

	std::vector<Client*> clients(max_clients);//ToDo move it to argument of this function
	clients[0] = cl1;
	clients[1] = cl2;
	
	worldInfo *winfo = new worldInfo;
	winfo->clinfo = new std::vector<clientInfo*>(max_clients);
	winfo->active_clients = new std::vector<char>(max_clients);// 0 - not work, -1 - crashed, 1 - busy
	winfo->max_clients = max_clients;
	winfo->world = world;
	std::vector<clientInfo*>* clinfo = winfo->clinfo;
	for(int i = 0;i<max_clients;i++){
		(*clinfo)[i] = new clientInfo;
		(*clinfo)[i]->c = clients[i];
		(*clinfo)[i]->r = makeRobot(40,40, 100.0f + i*100.0f, 100.0f + i*100.0f, world);
		world->Add((*clinfo)[i]->r);
		
		(*winfo->active_clients)[i] = true;
		(*clinfo)[i]->isWorking = &(*winfo->active_clients)[i];
	}
	
	CreateThread(NULL, NULL, worldThread, winfo, NULL, NULL);
	
}
WorldHandler::~WorldHandler(void)
{
}
