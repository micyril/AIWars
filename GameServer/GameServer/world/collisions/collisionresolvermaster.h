#pragma once

#include <map>
#include "../mapelement.h"
#include "resolvers/collisionresolver.h"

class CollisionResolverMaster {
private:
	std::map<std::string, CollisionResolver*> mapElementsTypesToCollisionResolver;
	static CollisionResolverMaster crm;
	static bool areResolversInitialized;

	CollisionResolverMaster() {}

public:
	~CollisionResolverMaster();
	static void AddNewCollisionResolver(std::string updatedElementType, std::string collidedElementType, CollisionResolver *collisionResolver);
	static void Resolve(MapElement *updatedElement, MapElement *collidedElement);
};