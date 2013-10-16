#pragma once

#include <map>
#include "../mapelement.h"
#include "resolvers/collisionresolver.h"

class CollisionResolverMaster {
private:
	std::map<std::string, CollisionResolver*> mapElementsTypesToCollisionResolver;
	static CollisionResolverMaster crm;

public:
	~CollisionResolverMaster();
	static void AddNewCollisionResolver(std::string mapElementType1, std::string mapElementType2, CollisionResolver *collisionResolver);
	static void Resolve(MapElement *mapElement1, MapElement *mapElement2);
};