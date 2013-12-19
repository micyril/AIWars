#include <sstream>
#include "mapelement.h"

using namespace std;


int MapElement::lastId = -1;

/*MapElement::MapElement(std::string &viewType, int width, int height, float x, float y, float rotation, int layer) : 
	viewType(viewType), Rectangle(width, height, x, y, rotation), layer(layer) { defineId(); }
	*/
MapElement::MapElement(std::string &viewType, std::string collidedElementType, int width, int height, Point p, float rotation, int layer) : 
	viewType(viewType), collidedElementType(collidedElementType), Rectangle(width, height, p, rotation), layer(layer) { defineId(); }

MapElement::MapElement(std::string &viewType, std::string collidedElementType, int width, int height, Point p, Point rotationCenter, float rotation, int layer) :
	viewType(viewType), collidedElementType(collidedElementType), Rectangle(width, height, p, rotationCenter, rotation), layer(layer) { defineId(); }

std::string MapElement::serializeWithoutBrackets() {
	stringstream stream;
	stream << Rectangle::serializeWithoutBrackets() << "," << "\"layer\": " << 
		layer << ","  << "\"type\": \"" << viewType << "\", " << "\"id\": " << id;
	return stream.str();
}

void MapElement::defineId() {
	id = ++lastId;
}

void MapElement::Move(Point delta) {
	vertice = vertice + delta;
	rotationCenter = rotationCenter + delta;
}

void MapElement::Move(float distance) {
	Move(Point(distance * cos(getAngle()), distance * sin(-getAngle())));
}

void MapElement::Rotate(float angle) {
	setAngle(getAngle() + angle);
}