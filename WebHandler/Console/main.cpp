#include "..\WebHandler\WebHandler.h"
#include "..\WebHandler\Utils.h"
#include "..\WebHandler\TestData.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void OnConnect(Client *c) {
	c->sendSelfInfo();
	Sleep(2000);
	c->sendEnemyInfo(20);
	Sleep(1000);
	c->notifyStart();
}

int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE);

	return 0;
}
