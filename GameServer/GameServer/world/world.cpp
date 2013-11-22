#include <sstream>
#include <iostream>
#include "world.h"
#include "robot\robot.h"
#include "..\exceptions.h"

using namespace std;

World::World(int width, int height) : width(width), height(height) {}

World::World(int width, int height, std::list<WorldObject*>& objects) : objects(objects), width(width), height(height) {
	for(auto it = objects.begin(); it != objects.end(); it++)
		for(auto mapElemIt = (*it)->mapElements.begin(); mapElemIt != (*it)->mapElements.end(); mapElemIt++)
			mapElements.push_back(*mapElemIt);
}

World::~World()
{
	for(auto it = objects.begin(); it != objects.end(); it++)
		delete *it;
}

void World::Update(float delta) {
	worldUpdateMutex.lock();
	for(auto it = objects.begin(); it != objects.end(); it++)
		(*it)->Update(delta);
	worldUpdateMutex.unlock();
}

void World::Add(WorldObject *worldObject) {
	worldUpdateMutex.lock();
	objects.push_back(worldObject);
	for(auto mapElemIt = worldObject->mapElements.begin(); mapElemIt != worldObject->mapElements.end(); mapElemIt++)
		mapElements.push_back(*mapElemIt);
	worldUpdateMutex.unlock();
}

void World::Delete(WorldObject *worldObject) {
	worldUpdateMutex.lock();
	for(auto mapElemIt = worldObject->mapElements.begin(); mapElemIt != worldObject->mapElements.end(); mapElemIt++)
		mapElements.remove(*mapElemIt);
	objects.remove(worldObject);
	delete worldObject;
	worldUpdateMutex.unlock();
}

std::string World::Serialize() {
	stringstream stream;
	stream <<	"{" <<
				"\"width\":" << width << ",\"height\":" << height << "," <<
				"\"viewsize\": " << 0 << ",\"viewangle\": " << 0; // TODO: записать сюда размеры видимой части
	stream << "}";

	return stream.str();
}