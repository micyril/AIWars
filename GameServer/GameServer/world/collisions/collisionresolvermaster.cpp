#include "collisionresolvermaster.h"
#include "exceptions.h"
#include "resolvers\collisionresolvers.h"

CollisionResolverMaster CollisionResolverMaster::crm;
bool CollisionResolverMaster::areResolversInitialized = false;

CollisionResolverMaster::~CollisionResolverMaster() {
	for(std::map<std::string, CollisionResolver* >::iterator it = mapElementsTypesToCollisionResolver.begin(); it != mapElementsTypesToCollisionResolver.end(); it++)
		delete it->second;
}

void CollisionResolverMaster::AddNewCollisionResolver(std::string updatedElementType, std::string collidedElementType, CollisionResolver *collisionResolver) {
	crm.mapElementsTypesToCollisionResolver.insert(std::pair< std::string, CollisionResolver* >(updatedElementType + " " + collidedElementType, collisionResolver));
}

void CollisionResolverMaster::Resolve(MapElement *updatedElement, MapElement *collidedElement) {
	if(areResolversInitialized == false) {
		defineCollisionResolvers();
		areResolversInitialized = true;
	}

	std::map<std::string, CollisionResolver* >::iterator it = 
		crm.mapElementsTypesToCollisionResolver.find(updatedElement->GetType() + " " + collidedElement->GetType());

	if (it == crm.mapElementsTypesToCollisionResolver.end())
		throw CollisionResolverNotFoundException(updatedElement->GetType(), collidedElement->GetType());

	it->second->Resolve(updatedElement, collidedElement);
}