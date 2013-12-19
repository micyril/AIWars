#include "bulletcollisionresolver.h"
#include "../collisionresolvermaster.h"
#include "../../robot/robot.h"
#include "../../robot/components/gun/bullet.h"

BulletCollisionResolver* BulletCollisionResolver::bulletCollisionResolver = NULL;

BulletCollisionResolver::BulletCollisionResolver() {
	CollisionResolverMaster::AddNewCollisionResolver(BulletBody::GetClassType(), RobotMapElement::GetClassType(), this);
	CollisionResolverMaster::AddNewCollisionResolver(RobotMapElement::GetClassType(), BulletBody::GetClassType(), this);
	CollisionResolverMaster::AddNewCollisionResolver(BulletBody::GetClassType(), BulletBody::GetClassType(), this);
}

void BulletCollisionResolver::Resolve(MapElement *updatedElement, MapElement *collidedElement) {
	if(updatedElement->GetType() == BulletBody::GetClassType() && collidedElement->GetType() == RobotMapElement::GetClassType())
		resolve((BulletBody*)updatedElement, (RobotMapElement*)collidedElement);
	else if(updatedElement->GetType() == BulletBody::GetClassType() && collidedElement->GetType() == BulletBody::GetClassType())
		resolve((BulletBody*)updatedElement, (BulletBody*)collidedElement);
}

void BulletCollisionResolver::Initilize() {
	if(bulletCollisionResolver == NULL)
		bulletCollisionResolver = new BulletCollisionResolver();
}

void BulletCollisionResolver::resolve(BulletBody *updatedElement, RobotMapElement *collidedElement) { 
	if(updatedElement->IsDestroyed() == false) {
		collidedElement->Damage(updatedElement->GetDamageInfluence());
		updatedElement->Destroy();
	}
}

void BulletCollisionResolver::resolve(BulletBody *updatedElement, BulletBody *collidedElement) {
	updatedElement->Destroy();
	collidedElement->Destroy();
}