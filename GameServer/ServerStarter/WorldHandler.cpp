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
	std::string ack = "ACK\r\n";//ToDo move it to constants
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
        }
		else{
			std::string answer = eog;//ToDo just for test, get rip of that
			iSendResult = send( info->c->commandSocket, answer.data(), answer.size(), 0 );
			closesocket(info->c->commandSocket); 
		}
    }
}
#include "../GameServer/world/collisions/collisionchecker.h"
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
		//system("cls");
		if(CollisionChecker::Check(info->c1info->r->frame, info->c2info->r->frame))
			cerr << "collision" << endl;
		end = clock();
		sleepPeriod = end - start;
	}
	
	//info.cl1->notifyFinish();
	//info.cl2->notifyFinish();
	return 0;
}


void WorldHandler::startGame(Client* cl1, Client* cl2){
	
	World* world = new World(500, 500);
	Robot* r1 = makeRobot(40,40, 100.0f, 100.0f, world);
	Robot* r2 = makeRobot(40,40, 200.0f, 200.0f, world);
	world->Add(r1);
	world->Add(r2);
	


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
