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

void BulletCollisionResolver::Resolve(MapElement *subject, MapElement *object) {
	if(subject->GetType() == BulletBody::GetClassType() && object->GetType() == RobotMapElement::GetClassType())
		resolve((BulletBody*)subject, (RobotMapElement*)object);
	else if(subject->GetType() == BulletBody::GetClassType() && object->GetType() == BulletBody::GetClassType())
		resolve((BulletBody*)subject, (BulletBody*)object);
}

void BulletCollisionResolver::Initilize() {
	if(bulletCollisionResolver == NULL)
		bulletCollisionResolver = new BulletCollisionResolver();
}

void BulletCollisionResolver::resolve(BulletBody *subject, RobotMapElement *object) { 
	if(subject->IsDestroyed() == false) {
		object->Damage(subject->GetDamageInfluence());
		subject->Destroy();
	}
}

void BulletCollisionResolver::resolve(BulletBody *subject, BulletBody *object) {
	subject->Destroy();
	object->Destroy();
}