#pragma once

#include <string>
#include "../../../WebHandler/WebHandler/Serializable.h"

class Rectangle : public Serializable {
public:
	int width;
	int height;
	int x;
	int y;
	int rotation;

	Rectangle(int width, int height, int x, int y, int rotation = 0);
	virtual std::string Serialize();

protected:
	virtual std::string serializeWithoutBrackets();
};

class MapElement : public Rectangle {
public:
	int layer;

	MapElement(int width, int height, int x, int y, int rotation = 0, int layer = 0);
	
	//todo: use safe pointers 
	//virtual void ApplyCollisionEffect(MapElement *mapElement) = 0;

protected:
	virtual std::string serializeWithoutBrackets();
};