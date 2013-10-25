#include "Application.h"
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
	m.lock();
	clients[c->id] = c;
	if(clientsToRun[c->id] == NULL){
		int otherClient = waitingClients.front()->id;
		clientsToRun[c->id] = otherClient;
		clientsToRun[otherClient] = c->id;
	}
	else
	m.unlock();
}
void Application::listenOnPort(int port){
	WorldHandler wh;
	 WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

  
    // Create a SOCKET for connecting to server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; 
	addr.sin_port = htons(port);
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket == INVALID_SOCKET) {
        /*printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;*/
		throw SocketConnectionExeption();
    }

    // Setup the TCP listening socket
    iResult = ::bind( ListenSocket, (sockaddr*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
       /* printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;*/
		throw SocketConnectionExeption();
    }


    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
       /* printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;*/
		throw SocketConnectionExeption();
    }
	char buff[SIZE_BUFFER];
    // Accept a client socket
	while(1){
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
		   /* printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;*/
			throw SocketConnectionExeption();
		}
		
		recv(ClientSocket, buff, SIZE_BUFFER, 0);
		int idClient;
		itoa(idClient, buff, 10);
		//TODO check id for exist
		clients[idClient]->commandSocket = ClientSocket;
		isClientConnect.insert(idClient);
		if(isClientConnect.find(clientsToRun[idClient]) != isClientConnect.end())
			wh.start(clients[idClient], clients[clientsToRun[idClient]]);

        
	}
}

static void Run(){
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&Application::onConnect);
	listen();
}

Application::~Application(void)
{
}
