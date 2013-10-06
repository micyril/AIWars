#include "..\WebHandler\WebHandler.h"
#include "..\WebHandler\Utils.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void OnConnect(Client *c) {
	c->sendData("Hello", 6);
	Sleep(3000);
	c->sendData("world!", 6);
	Sleep(3000);
	c->close();
}

int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}