#pragma once

#include <string>
#include "../../../WebHandler/WebHandler/Serializable.h"
#include "../math/math.h"

class MapElement : public Rectangle {
public:
	int layer;

	//MapElement(std::string &viewType, int width, int height, float x, float y, float rotation = 0, int layer = 0);
	MapElement(std::string &viewType, int width, int height, Point p, float rotation = 0, int layer = 0);
	//MapElement(std::string &viewType, int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation, int layer = 0);
	MapElement(std::string &viewType, int width, int height, Point p, Point rotationCenter, float rotation, int layer = 0);
	virtual ~MapElement() {}
	virtual std::string GetType() = 0;

	virtual void Move(Point delta);
	void Move(float distance);
	virtual void Rotate(float angle);

protected:
	std::string viewType;
	virtual std::string serializeWithoutBrackets();

private:
	int id;
	static int lastId;

	void defineId();
};