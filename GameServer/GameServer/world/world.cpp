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
	stream << "{";
	stream << "\"mtype\":\"game_info\","; // <-- тип сообщения
	stream << "\"width\":" << width << ",";
	stream << "\"height\":" << height << ",";
	stream << "\"mapelements\":["; // для массивов квадратные скобки, для объектов - фигурные !

	std::list<MapElement*>::iterator it = mapelements.begin();
	stream << (*it)->Serialize();
	it++;
	for(; it != mapelements.end(); it++)
		stream << "," << (*it)->Serialize();
	stream << "]";
	stream << "}";

	/*
	stream << "\"world\" : " << endl << "{" << endl;
	stream << "\"width\" : " << this->width << "," << endl;
	stream << "\"height\" : " << this->height << "," << endl;
	stream << "\"mapelements\" : " << endl << "{" << endl;

	std::list<MapElement*>::iterator it = mapelements.begin();
	stream << (*it)->Serialize();
	it++;
	for(; it != mapelements.end(); it++)
		stream << "," << endl << (*it)->Serialize();

	stream << endl << "}" << endl;
	stream << "}";
	*/
	return stream.str();
}

World::~World()
{
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		delete *it;
	for(std::list<MapElement*>::iterator it = mapelements.begin(); it != mapelements.end(); it++)
		delete *it;
}