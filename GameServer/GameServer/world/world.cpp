#include <sstream>
#include <iostream>
#include "world.h"
#include "robot\robot.h"
#include "..\exceptions.h"
#include "collisions\collisionchecker.h"
#include "collisions\collisionresolvermaster.h"

using namespace std;

World::World(int width, int height) : width(width), height(height) {}

World::World(int width, int height, std::list<WorldObject*>& objects) : objects(objects), width(width), height(height) {
	for(auto it = objects.begin(); it != objects.end(); it++)
		for(auto mapElemIt = (*it)->mapElements.begin(); mapElemIt != (*it)->mapElements.end(); mapElemIt++)
			mapElements.push_back(*mapElemIt);
}

World::~World() {
	for(auto it = objects.begin(); it != objects.end(); it++)
		delete *it;
}

void World::Update(float delta) {
	for(auto objectIt = objects.begin(); objectIt != objects.end(); objectIt++) {
		(*objectIt)->Update(delta);
		checkAndResolveCollisionsFor(*objectIt);
	}

	deleteObjects();
	addObjects();
}

void World::Add(WorldObject *worldObject) {
	mutexForAdding.lock();
	objectsForAdding.push(worldObject);
	mutexForAdding.unlock();
}

void World::Delete(WorldObject *worldObject) {
	mutexForDeleting.lock();
	objectsForDeleting.push(worldObject);
	mutexForDeleting.unlock();
}

std::string World::Serialize() {
	stringstream stream;
	stream <<	"{" <<
				"\"width\":" << width << ",\"height\":" << height << "," <<
				"\"viewsize\": " << 0 << ",\"viewangle\": " << 0; // TODO: записать сюда размеры видимой части
	stream << "}";

	return stream.str();
}

void World::checkAndResolveCollisionsFor(WorldObject *updatedObject) {
	for (auto updatedObjectMEIt = updatedObject->mapElements.begin(); updatedObjectMEIt != updatedObject->mapElements.end(); updatedObjectMEIt++)
		for (auto mapElementIt = mapElements.begin(); mapElementIt != mapElements.end();  mapElementIt++)
			if(*updatedObjectMEIt != *mapElementIt && CollisionChecker::Check(*updatedObjectMEIt, *mapElementIt))
				CollisionResolverMaster::Resolve(*updatedObjectMEIt, *mapElementIt);
}

void World::deleteObjects() {
	mutexForDeleting.lock();
	while(objectsForDeleting.size() > 0) {
		WorldObject *worldObject = objectsForDeleting.front();
		for(auto mapElemIt = worldObject->mapElements.begin(); mapElemIt != worldObject->mapElements.end(); mapElemIt++)
			mapElements.remove(*mapElemIt);
		objects.remove(worldObject);
		objectsForDeleting.pop();
		delete worldObject;
	}
	mutexForDeleting.unlock();
}

void World::addObjects() { 
	mutexForAdding.lock();
	while(objectsForAdding.size() > 0) {
		WorldObject *worldObject = objectsForAdding.front();
		objects.push_back(worldObject);
		for(auto mapElemIt = worldObject->mapElements.begin(); mapElemIt != worldObject->mapElements.end(); mapElemIt++)
			mapElements.push_back(*mapElemIt);
		objectsForAdding.pop();
	}
	mutexForAdding.unlock();
}