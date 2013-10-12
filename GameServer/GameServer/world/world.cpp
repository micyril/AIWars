#include "world.h"

void World::Update(float delta) {
	for(std::list<MapObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->Update(delta);
}