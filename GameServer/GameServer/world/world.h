#pragma once

#include <list>
#include <queue>
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
	std::queue<WorldObject*> objectsForAdding;
	std::queue<WorldObject*> objectsForDeleting;
	std::mutex mutexForAdding;
	std::mutex mutexForDeleting;

	void checkAndResolveCollisionsFor(WorldObject *updatedObject);
	void deleteObjects();
	void addObjects();

public:
	World(int width, int height);
	World(int width, int height, std::list<WorldObject*>& objects);
	~World();
	void Update(float delta);
	void Add(WorldObject *worldObject);
	void Delete(WorldObject *worldObject);
	virtual std::string Serialize();

	const std::list<MapElement*>* GetMapElements() const {
		return &mapElements;
	}
};