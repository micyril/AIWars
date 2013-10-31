#include "WorldHandler.h"
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
Robot* WorldHandler::makeRobot(){
	RobotFrame *robotFrame = new RobotFrame(50, 50, 200, 200);
	float movingSpeed = 40;
	float rotationSpeed = 0.005;
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
	std::getline(ss, argsstr, ' ');
	arg = argsstr.data();
}
DWORD WINAPI clientThread(LPVOID lpParam){

	clientInfo* info = (clientInfo*)lpParam;

	int iResult;
	int iSendResult;
    char recvbuf[1024];
    int recvbuflen = 1024;
	std::string command("");
	std::string arg;
	send(info->c->commandSocket, "ACK", 3, 0);
    do {
		iResult = recv(info->c->commandSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
			parceCommand(recvbuf, iResult, command, arg);
			info->r->Execute(command, arg);  //TODO: catch exceptions
            iSendResult = send( info->c->commandSocket, "ACK", 3, 0 );//ToDo replace for retur of sendCommandToRobot
            if (iSendResult == SOCKET_ERROR) {
				throw NotImplementedException();  //TODO: it's not good to use this type of exception here
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
			fprintf(stderr, "Connection closing...\n");
        else  {
         
			continue;
        }

    } while (iResult > 0);
}
DWORD WINAPI worldThread( LPVOID lpParam ){
	worldInfo* info = (worldInfo*)lpParam;
	int sleepPeriod = 20;
	info->c1info->c->sendSelfInfo();
	Sleep(2000);
	info->c1info->c->sendEnemyInfo(info->c2info->c->id);
	Sleep(1000);
	info->c1info->c->sendGameInfo(info->world);
	info->c1info->c->notifyStart();

	info->c2info->c->sendSelfInfo();
	Sleep(2000);
	info->c2info->c->sendEnemyInfo(info->c1info->c->id);
	Sleep(1000);
	info->c2info->c->sendGameInfo(info->world);
	info->c2info->c->notifyStart();
	CreateThread(NULL, NULL, clientThread, info->c1info, NULL, NULL);
	CreateThread(NULL, NULL, clientThread, info->c2info, NULL, NULL);
	while(true) {
		Sleep(sleepPeriod);
		info->world->Update(sleepPeriod / 1000.0);
		info->c1info->c->notifyUpdate(info->world->getElements());
		info->c2info->c->notifyUpdate(info->world->getElements());
	}
	
	//info.cl1->notifyFinish();
	//info.cl2->notifyFinish();
	return 0;
}


void WorldHandler::startGame(Client* cl1, Client* cl2){
	
	Robot* r1 = makeRobot();
	Robot* r2 = makeRobot();

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
	//TODO delete wordls
}
