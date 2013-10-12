#pragma once

class Rectangle {
};

class MapElement : public Rectangle {
public:
	//todo: use safe pointers
	virtual void ApplyCollisionEffect(MapElement *mapElement) = 0;
};