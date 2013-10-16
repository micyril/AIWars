#include <sstream>
#include <iostream>
#include "world.h"
#include "robot\robot.h"
#include "collisions\collisions.h"
#include "..\exceptions.h"

using namespace std;

World::World(int w, int h) : width(w), height(h) {
	MapElement*  testElement =  new RobotFrame(5, 13, NULL); //testing
	this->mapelements.push_back(testElement);
	MapElement*  testElement1 =  new RobotFrame(6, 12, NULL); //testing
	this->mapelements.push_back(testElement1);
	DEFINE_COLLISION_RESOLVERS;
	try {
		CollisionResolverMaster::Resolve(testElement, testElement1);
	}
	catch (NotImplementedException) {
		std::cout << "It's working!" << std::endl;
	}
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