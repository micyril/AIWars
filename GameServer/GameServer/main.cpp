#include <iostream>
#include "world\mapelement.h"
#include "world\world.h"
int test()
{
	World w(500,500);
	std::cout << w.Serialize();
	return 0;
}
int main() {
	return test();
}