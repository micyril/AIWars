#include "WorldHandler.h"
#include <iostream>
#include "time.h"
#define MAX_CONNECTIONS (100)

WorldHandler::WorldHandler(void)
{
	/*InitializeThreadpoolEnvironment(&pool_env);
	pool = CreateThreadpool(0);		
	SetThreadpoolThreadMinimum(pool, MAX_CONNECTIONS);
	SetThreadpoolThreadMaximum(pool, MAX_CONNECTIONS);
	SetThreadpoolCallbackPool(&pool_env, pool);*/

	worldId = 0;
}
Robot* WorldHandler::makeRobot(int width, int hieght, float x, float y){
	RobotFrame *robotFrame = new RobotFrame(width, hieght, x, y);
	float movingSpeed = 10.0f;
	float rotationSpeed = 1.0f;
	RobotComponent *runningGear = new RunningGear(movingSpeed, rotationSpeed);
	std::list<RobotComponent*> robotComponents;
	robotComponents.push_back(runningGear);
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
	std::string ack = "ACK\r\n";
	std::string eog = "EOG\r\n";
	send(info->c->commandSocket, ack.data(), ack.length(), 0);
     while (1) {
		iResult = recv(info->c->commandSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
			if (strcmp(recvbuf, "EOG") == 0){
				std::stringstream ss;
				std::string answer = info->r->Execute(command, arg); 
				answer = eog;//ToDo just for test, get rip of that
				iSendResult = send( info->c->commandSocket, answer.data(), answer.size(), 0 );
				std::cerr <<  "End...\n";
				break;
			}
			parceCommand(recvbuf, iResult, command, arg);
			
			try{
				std::string answer = info->r->Execute(command, arg); 
				answer = ack;//ToDo just for test, get rip of that
				iSendResult = send( info->c->commandSocket, answer.data(), answer.size(), 0 );
			}
			catch(NotSupportedCommandException &ex){
				cerr << ex.what() << endl;

			}
			catch(...){
				std::cerr << "FAIL" << std::endl;
			}
            if (iSendResult == SOCKET_ERROR) {
				throw SocketConnectionException(); 
            }
            cerr << "Bytes sent: " << iSendResult << endl;;
        }	
    }
}
DWORD WINAPI worldThread( LPVOID lpParam ){
	worldInfo* info = (worldInfo*)lpParam;
	int sleepPeriod = 20;

	info->c1info->c->sendSelfInfo();
	info->c1info->c->sendEnemyInfo(info->c2info->c->id);
	info->c1info->c->sendGameInfo(info->world);
	info->c2info->c->sendSelfInfo();
	info->c2info->c->sendEnemyInfo(info->c1info->c->id);
	info->c2info->c->sendGameInfo(info->world);


	info->c1info->c->notifyStart();
	info->c2info->c->notifyStart();
	CreateThread(NULL, NULL, clientThread, info->c1info, NULL, NULL);
	CreateThread(NULL, NULL, clientThread, info->c2info, NULL, NULL);
	Sleep(sleepPeriod);
	clock_t start, end;
	while(true) {
		start = clock();
		Sleep(20);//dont work without
		info->world->Update(sleepPeriod / 1000.0f);
		info->c1info->c->notifyUpdate(info->world->getElements());
		info->c2info->c->notifyUpdate(info->world->getElements());
		end = clock();
		sleepPeriod = end - start;
	}
	
	//info.cl1->notifyFinish();
	//info.cl2->notifyFinish();
	return 0;
}


void WorldHandler::startGame(Client* cl1, Client* cl2){
	
	Robot* r1 = makeRobot(40,40, 100.0f, 100.0f);
	Robot* r2 = makeRobot(40,40, 200.0f, 200.0f);

	std::list<WorldObject*> worldObjects;
	worldObjects.push_back(r1);
	worldObjects.push_back(r2);

	World* world = new World(500, 500, worldObjects);


	this->worlds[worldId] = world;
	worldId++;
	cl1->worldId = worldId;
	cl2->worldId = worldId;

	worldInfo *winfo = new worldInfo;

	clientInfo* c1info = new clientInfo;
	c1info->c = cl1;
	c1info->r = r1;

	clientInfo* c2info = new clientInfo;
	c2info->c = cl2;
	c2info->r = r2;

	winfo->c1info = c1info;
	winfo->c2info = c2info;
	winfo->world = world;

	CreateThread(NULL, NULL, worldThread, winfo, NULL, NULL);
}
WorldHandler::~WorldHandler(void)
{
	for(std::map<int, World*>::iterator it = worlds.begin();it!= worlds.end();it++ )
		delete it->second;
}
