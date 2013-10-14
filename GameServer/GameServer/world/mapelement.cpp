#include <sstream>
#include "mapelement.h"

using namespace std;

Rectangle::Rectangle(int width, int height, int x, int y, int rotation) :
		width(width), height(height), x(x), y(y), rotation(rotation) {}

std::string Rectangle::serializeWithoutBrackets() {
	stringstream stream;
	stream <<	"\"width\" : "		<< this->width		<< "," << endl << 
				"\"height\" : "		<< this->height   	<< "," << endl << 
				"\"x\" : "			<< this->x		  	<< "," << endl <<
				"\"y\" : "			<< this->y		  	<< "," << endl <<
				"\"rotation\" : "	<< this->rotation;
	return stream.str();
}

std::string Rectangle::Serialize() {
	stringstream stream;
	stream << "{" << endl << serializeWithoutBrackets() << endl << "}";
	return stream.str();
}

MapElement::MapElement(int width, int height, int x, int y, int rotation, int layer) : 
		Rectangle(width, height, x, y, rotation), layer(layer) {}

std::string MapElement::serializeWithoutBrackets() {
	stringstream stream;
	stream << Rectangle::serializeWithoutBrackets() << "," << endl <<
			  "\"layer\" : " << this->layer;
	return stream.str();
}