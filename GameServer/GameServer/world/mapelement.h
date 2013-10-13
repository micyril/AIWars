#pragma once

#include <string>
#include "../../../WebHandler/WebHandler/Serializable.h"

class Rectangle {
public:
	int width;
	int height;
	int x;
	int y;

	int rotation;

	Rectangle(int width, int height, int x, int y, int rotation = 0) :
		width(width), height(height), x(x), y(y), rotation(rotation) {}
};

class MapElement : public Rectangle, Serializable {
public:
	int layer;

	MapElement(int width, int height, int x, int y, int rotation = 0, int layer = 0) : 
		Rectangle(width, height, x, y, rotation), layer(layer) {}
	
	//todo: use safe pointers 
	//virtual void ApplyCollisionEffect(MapElement *mapElement) = 0;
	virtual std::string Serialize();
};