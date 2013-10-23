#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "ClientHandler.h"

#pragma comment (lib, "Ws2_32.lib")
//LISTENONPORT – Listens on a specified port for incoming connections 
//or data
PCSTR intToPCSTR(int portno){
	std::stringstream ss;
	ss << portno;
	return (PCSTR)ss.str().c_str();
}
void ClientHandler::InitSocket(const char* portno)
{
    WSADATA wsaData;
    int iResult;

    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

  
   
	printf("test\n");
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
		throw SocketConnectionException();
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, (sockaddr*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
       /* printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;*/
		throw SocketConnectionException();
    }


    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
       /* printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;*/
		throw SocketConnectionException();
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
       /* printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;*/
		throw SocketConnectionException();
    }

    // No longer need server socket
    closesocket(ListenSocket);

    

    // shutdown the connection since we're done
   
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void ClientHandler::CloseConnection (){
    //Close the socket if it exists
  

    // cleanup
    closesocket(ClientSocket);
}




ClientHandler::ClientHandler(Robot* robotsForExecute, const char* port){
	this->robot = robotsForExecute;
	InitSocket(port);
}


void* ClientHandler::SendCommandToRobot(std::string command, void* args){
	return robot->Execute(command, args);
}
void ClientHandler::parceCommand(char* input, int size, std::string &command, void* args){
	std::string instr(input, input + size);
	std::stringstream ss(instr);
	std::string index("");
	command = "";
	std::string argsstr("");
	std::getline(ss, index, '_');//skip index
	std::getline(ss, command, '_');
	std::getline(ss, argsstr, '_');
	args = (void*)argsstr.data();
}
void ClientHandler::Execute(){
	// Receive until the peer shuts down the connection
	int iResult;
	int iSendResult;
    char recvbuf[1024];
    int recvbuflen = 1024;
	std::string command("");
	void* args = NULL;
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
			parceCommand(recvbuf, iResult, command, args);
			SendCommandToRobot(command, args);
            iSendResult = send( ClientSocket, "ACK", 3, 0 );//ToDo replace for retur of sendCommandToRobot
            if (iSendResult == SOCKET_ERROR) {
                /*printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;*/
				throw SocketConnectionException();
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
          /*  printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;*/
			throw SocketConnectionException();
        }

    } while (iResult > 0);
}



ClientHandler::~ClientHandler(void){
	this->CloseConnection();
}
