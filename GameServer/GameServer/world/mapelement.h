#pragma once

#include <string>
#include "../../../WebHandler/WebHandler/Serializable.h"
#include "../math/math.h"

class MapElement : public Rectangle {
public:
	int layer;

	MapElement(std::string &viewType, std::string collidedElementType, int width, int height, Point p, float rotation = 0, int layer = 0);
	MapElement(std::string &viewType, std::string collidedElementType, int width, int height, Point p, Point rotationCenter, float rotation, int layer = 0);
	virtual ~MapElement() {}
	virtual void Move(Point delta);
	void Move(float distance);
	virtual void Rotate(float angle);

	const std::string& GetViewType() const {
		return viewType;
	}

	const std::string& GetCollidedElementType() const {
		return collidedElementType;
	}

protected:
	virtual std::string serializeWithoutBrackets();

private:
	int id;
	static int lastId;
	std::string viewType;
	std::string collidedElementType;

	void defineId();
};