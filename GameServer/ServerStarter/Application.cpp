#include "Application.h"
#include <iostream>
#define SIZE_BUFFER 16

std::list<Client*> Application::waitingClients;
std::mutex Application::m;
std::map<int, Client*> Application::clients;
std::set<int> Application::isClientConnect;
std::map<int, int> Application::clientsToRun;


Application::Application(void)
{
}
void Application::onConnect(Client* c){
	std::cerr << "client connected" << std::endl;
	m.lock();
	clients[c->id] = c;
	c->sendSelfInfo();
	if(clientsToRun.find(c->id) == clientsToRun.end()){
		if(!waitingClients.empty()){
			int otherClient = waitingClients.front()->id;
			clientsToRun[c->id] = otherClient;
			clientsToRun[otherClient] = c->id;
			waitingClients.erase(waitingClients.begin());
		}
		else{
			waitingClients.push_back(c);
		}
	}
	m.unlock();
	std::cerr << "client added" << std::endl;
}
void Application::listenOnPort(int port){
	WorldHandler wh;
	 WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

  
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; 
	addr.sin_port = htons(port);
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);



    iResult = ::bind( ListenSocket, (sockaddr*)&addr, sizeof(addr));
   


    iResult = listen(ListenSocket, SOMAXCONN);
    
	char buff[SIZE_BUFFER];

	while(1){
		if(clients.empty())
			continue;
		ClientSocket = accept(ListenSocket, NULL, NULL);
		
		//TODO check wrong value
		recv(ClientSocket, buff, SIZE_BUFFER, 0);
		int idClient = atoi(buff);
		std::cerr << "Client " << idClient << " is connected" << endl;
		if(clients.find(idClient) == clients.end())
			continue;
		clients[idClient]->commandSocket = ClientSocket;
		isClientConnect.insert(idClient);
		std::cerr << "Client " << idClient << " waiting for partner..." << endl;
		if(clientsToRun.size() >= 2){
			std::set<int>::iterator it = isClientConnect.find(clientsToRun[idClient]);
			
			if(it != isClientConnect.end()){
				cerr << "start game " << idClient << " VS " << clientsToRun[idClient] << endl;
				wh.startGame(clients[idClient], clients[clientsToRun[idClient]]);
				std::set<int>::iterator it1 = isClientConnect.find(idClient);
				isClientConnect.erase(it1);
				isClientConnect.erase(it);

			}
		}

        
	}
}

void Application::Run(){
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&Application::onConnect);
	listenOnPort(2560);
}

Application::~Application(void)
{
}
