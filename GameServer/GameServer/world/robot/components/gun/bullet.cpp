#include "bullet.h"
#include "gun.h"
#include "../../../world.h"

std::string BulletBody::type = "BulletBody";

BulletBody::BulletBody(int damageInfluence, int width, int height, Point p, Point rotationCenter, float rotation) : 
	MapElement(std::string("Bullet"), width, height, p, rotationCenter, rotation), damageInfluence(damageInfluence), isDestroyed(false) {}

int BulletBody::GetDamageInfluence() {
	return damageInfluence;
}

void BulletBody::Destroy() {
	isDestroyed = true;
}

bool BulletBody::IsDestroyed() {
	return isDestroyed;
}

std::string BulletBody::GetType() {
	return GetClassType();
}

std::string BulletBody::GetClassType() {
	return type;
}

Bullet::Bullet(BulletBody *bulletBody, World *world, float movingSpeed) : body(bulletBody), world(world), movingSpeed(movingSpeed) {
	mapElements.push_back(body);
}

void Bullet::Update(float delta) {
	if(body->IsDestroyed()) {
		world->Delete(this);
		return;
	}
	body->Move(movingSpeed * delta);
}

BulletFactory::BulletFactory(GunBarrel *barrel, World *world, float bulletsSpeed, int bulletsDamageInfluence) : 
	barrel(barrel), world(world), bulletsSpeed(bulletsSpeed), bulletsDamageInfluence(bulletsDamageInfluence) {}

Bullet *BulletFactory::Create() {
	BulletBody* bulletBody = new BulletBody(bulletsDamageInfluence, barrel->width / 5, barrel->height, barrel->vertice + Point(barrel->width + 0.001F, 0.0f), 
		barrel->rotationCenter, barrel->getAngle());
	return new Bullet(bulletBody, world, bulletsSpeed);
}