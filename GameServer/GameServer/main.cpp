#include <iostream>
#include "world\mapelement.h"
#include "world\world.h"
#include "..\..\WebHandler\WebHandler\WebHandler.h"

void OnConnect(Client *c) {
	World w(500, 500);

	c->sendSelfInfo();
	Sleep(2000);
	c->sendEnemyInfo(20);
	Sleep(1000);
	c->sendGameInfo(&w);
	c->notifyStart();	
	c->notifyUpdate(w.getElements());
	Sleep(2000);
	c->notifyFinish(c->id);
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