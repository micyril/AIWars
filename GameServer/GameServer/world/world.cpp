#include <sstream>
#include "world.h"

using namespace std;

World::World(int w, int h) : width(w), height(h) {
	MapElement*  testElement =  new MapElement(5, 13, 13, 13, 1, 3); //testing
	this->mapelements.push_back(testElement);
	MapElement*  testElement1 =  new MapElement(15, 113, 113, 113, 11, 13); //testing
	this->mapelements.push_back(testElement1);
}

void World::Update(float delta) {
	for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->Update(delta);
}

std::string World::Serialize() {
	stringstream stream;

	stream << "\"world\" : " << endl << "{" << endl;
	stream << "\"width\" : " << this->width << endl;
	stream << "\"height\" : " << this->height << endl;
	stream << "\"mapelements\" : " << endl << "{" << endl;

	int i = 0;
	for(std::list<MapElement*>::iterator it = mapelements.begin(); it != mapelements.end(); it++)
		stream << "\"mapelement" << i++ << "\" :"  << endl <<(*it)->Serialize() << endl;

	stream << "}" << endl;
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