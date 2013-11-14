#include "bullet.h"
#include "gun.h"

std::string BulletBody::type = "BulletBody";

BulletBody::BulletBody(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation) : 
	MapElement(width, height, x, y, rotationCenterX, rotationCenterY, rotation) {}

std::string BulletBody::GetType() {
	return GetClassType();
}

std::string BulletBody::GetClassType() {
	return type;
}

Bullet::Bullet(BulletBody *bulletBody, float movingSpeed) : body(bulletBody), movingSpeed(movingSpeed) {
	mapElements.push_back(body);
}

void Bullet::Update(float delta) {
	body->Move(movingSpeed * delta);
}

BulletFactory::BulletFactory(GunBarrel *barrel, float bulletsSpeed) : 
	barrel(barrel), bulletsSpeed(bulletsSpeed) {}

Bullet *BulletFactory::Create() {
	BulletBody* bulletBody = new BulletBody(barrel->width / 5, barrel->height, barrel->x + barrel->width, barrel->y, 
		barrel->rotationCenterX, barrel->rotationCenterY, barrel->rotation);
	return new Bullet(bulletBody, bulletsSpeed);
}