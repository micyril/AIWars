#include "..\WebHandler\WebHandler.h"
#include "..\WebHandler\Utils.h"
#include "..\WebHandler\TestData.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void OnConnect(Client *c) {
	Sleep(1000);
	TestData td;
	c->sendObject(td);
}

int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}