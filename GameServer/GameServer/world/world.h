#pragma once

#include <list>
#include <mutex>
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
	std::list<MapElement*> mapElements;
	std::mutex worldUpdateMutex;

public:
	World(int width, int height);
	World(int width, int height, std::list<WorldObject*>& objects);
	~World();
	void Update(float delta);
	void Add(WorldObject *worldObject);
	void Delete(WorldObject *worldObject);
	virtual std::string Serialize();

	std::list<Serializable*>* getElements() {
		return (std::list<Serializable*>*)&mapElements;
	}
};