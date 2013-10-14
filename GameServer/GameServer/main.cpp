#include <iostream>
#include "world\mapelement.h"
#include "world\world.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"

void OnConnect(Client *c) {
	c->sendSelfInfo();
	Sleep(2000);
	c->sendEnemyInfo(20);
	World w(500, 500);
	c->sendObject(w);
	Sleep(1000);
	c->notifyStart();
}

int test()
{
	World w(500,500);
	std::cout << w.Serialize();
	return 0;
}
int main() {
	HANDLE h = WebHandler::StartHttp();
	WebHandler::setOnConnectCallback(&OnConnect);
	WaitForSingleObject(h, INFINITE); // join к потоку

	return test();
}