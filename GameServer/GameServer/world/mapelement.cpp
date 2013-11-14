#include <sstream>
#include "mapelement.h"

using namespace std;

Rectangle::Rectangle(int width, int height, float x, float y, float rotation) : 
	width(width), height(height), x(x), y(y), rotation(rotation) {
		rotationCenterX = x + width / 2;
		rotationCenterY = y + height / 2;
}

Rectangle::Rectangle(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation) :
	width(width), height(height), x(x), y(y), rotationCenterX(rotationCenterX), rotationCenterY(rotationCenterY), rotation(rotation) {}

std::string Rectangle::serializeWithoutBrackets() {
	stringstream stream;
	stream <<	"\"position\": {\"x\": " << x << ", \"y\": " << y << "}," <<
				"\"width\": " << width << ", \"height\": " << height << "," <<
				"\"angle\": " << rotation << "," <<
				"\"rotationcenter\": {\"x\": " << rotationCenterX << ", \"y\": " << rotationCenterY << "}"; // TODO: записать сюда координаты центра поворота
	return stream.str();
}

std::string Rectangle::Serialize() {
	stringstream stream;
	stream << "{" << endl << serializeWithoutBrackets() << endl << "}";
	return stream.str();
}

MapElement::MapElement(int width, int height, float x, float y, float rotation, int layer) : 
	Rectangle(width, height, x, y, rotation), layer(layer) {}

MapElement::MapElement(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation, int layer) :
	Rectangle(width, height, x, y, rotationCenterX, rotationCenterY, rotation), layer(layer) {}

std::string MapElement::serializeWithoutBrackets() {
	stringstream stream;
	stream << Rectangle::serializeWithoutBrackets() << "," << endl <<
			  "\"layer\": " << this->layer;
	return stream.str();
}

void MapElement::Move(float deltaX, float deltaY) {
	x += deltaX;
	y += deltaY;
	rotationCenterX += deltaX;
	rotationCenterY += deltaY;
}

void MapElement::Move(float distance) {
	Move(distance * cos(rotation), distance * sin(-rotation));
}

void MapElement::Rotate(float angle) {
	rotation += angle;
}