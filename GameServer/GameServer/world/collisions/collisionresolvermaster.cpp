#include "collisionresolvermaster.h"
#include "exceptions.h"

CollisionResolverMaster CollisionResolverMaster::crm;

CollisionResolverMaster::~CollisionResolverMaster() {
	for(std::map<std::string, CollisionResolver* >::iterator it = mapElementsTypesToCollisionResolver.begin(); it != mapElementsTypesToCollisionResolver.end(); it++)
		delete it->second;
}

void CollisionResolverMaster::AddNewCollisionResolver(std::string mapElementType1, std::string mapElementType2, CollisionResolver *collisionResolver) {
	crm.mapElementsTypesToCollisionResolver.insert(std::pair< std::string, CollisionResolver* >(mapElementType1 + " " + mapElementType2, collisionResolver));
}

void CollisionResolverMaster::Resolve(MapElement *mapElement1, MapElement *mapElement2) {
	std::map<std::string, CollisionResolver* >::iterator it = 
		crm.mapElementsTypesToCollisionResolver.find(mapElement1->GetType() + " " + mapElement2->GetType());

	if (it == crm.mapElementsTypesToCollisionResolver.end())
		throw CollisionResolverNotFoundException(mapElement1->GetType(), mapElement2->GetType());

	it->second->Resolve(mapElement1, mapElement2);
}