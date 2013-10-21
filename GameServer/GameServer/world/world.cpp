#include <sstream>
#include <iostream>
#include "world.h"
#include "robot\robot.h"
#include "collisions\collisions.h"
#include "..\exceptions.h"

using namespace std;

World::World(int width, int height, std::list<WorldObject*>& objects, std::list<MapElement*>& mapelements) : 
	width(width), height(height), objects(objects), mapelements(mapelements) {
}

void World::Update(float delta) {
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->Update(delta);
}

std::string World::Serialize() {
	stringstream stream;
	stream <<	"{" <<
				"\"width\":" << width << ",\"height\":" << height << "," <<
				"\"viewsize\": " << 0 << ",\"viewangle\": " << 0; // TODO: записать сюда размеры видимой части
	stream << "}";

	return stream.str();
}

World::~World()
{
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		delete *it;
	for(std::list<MapElement*>::iterator it = mapelements.begin(); it != mapelements.end(); it++)
		delete *it;
}