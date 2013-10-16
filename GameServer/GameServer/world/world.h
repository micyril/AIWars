#pragma once

#include <list>
#include "worldobject.h"
#include "mapelement.h"
#include "..\..\..\WebHandler\WebHandler\Serializable.h"

class World : public Serializable {
private:
	int width;
	int height;
	//todo: borders
	//todo: use safe pointers
	std::list<WorldObject*> objects;
	std::list<MapElement*> mapelements;

public:
	World(int w, int h);
	void Update(float delta);
	virtual std::string Serialize();
	~World();

	std::list<Serializable*>* getElements() {
		return (std::list<Serializable*>*)&mapelements;
	}
};