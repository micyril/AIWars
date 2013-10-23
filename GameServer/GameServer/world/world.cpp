#include <sstream>
#include <iostream>
#include "world.h"
#include "robot\robot.h"
#include "..\exceptions.h"

using namespace std;

World::World(int width, int height, std::list<WorldObject*>& objects) : width(width), height(height), objects(objects) {
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		for(std::list<MapElement*>::iterator mapElemIt = (*it)->mapElements.begin(); mapElemIt != (*it)->mapElements.end(); mapElemIt++)
			mapElements.push_back(*mapElemIt);
}

World::~World()
{
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		delete *it;
	for(std::list<MapElement*>::iterator it = mapElements.begin(); it != mapElements.end(); it++)
		delete *it;
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