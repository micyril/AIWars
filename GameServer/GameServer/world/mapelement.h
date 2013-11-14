#pragma once

#include <string>
#include "../../../WebHandler/WebHandler/Serializable.h"

class Rectangle : public Serializable {
public:
	int width;
	int height;
	float x;
	float y;
	float rotationCenterX;
	float rotationCenterY;
	float rotation;

	Rectangle(int width, int height, float x, float y, float rotation = 0);
	Rectangle(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation = 0);
	virtual std::string Serialize();

protected:
	virtual std::string serializeWithoutBrackets();
};

class MapElement : public Rectangle {
public:
	int layer;

	MapElement(int width, int height, float x, float y, float rotation = 0, int layer = 0);
	MapElement(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation, int layer = 0);
	virtual ~MapElement() {}
	virtual std::string GetType() = 0;

	virtual void Move(float deltaX, float deltaY);
	void Move(float distance);
	virtual void Rotate(float angle);

protected:
	virtual std::string serializeWithoutBrackets();
};