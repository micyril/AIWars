#pragma once
#include "..\world\robot\robot.h"
#include "..\exceptions.h"
#include <windows.h>
#include <sstream>
#include <vector>
class ClientHandler{
	Robot* robot;
	std::string adress;
	int port;
	SOCKET ClientSocket;
	SOCKET ListenSocket;
	
	void InitSocket(const char* PortNo);
	void CloseConnection ();
	void parceCommand(char* input,int size, std::string &command, void* args);
	void *SendCommandToRobot(std::string command, void* args);
	void getCommand();
public:
	ClientHandler(Robot* robotForExecute, const char* port);
	void Execute();
	~ClientHandler(void);
};

