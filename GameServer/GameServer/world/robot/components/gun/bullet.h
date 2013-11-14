#pragma once

#include "../../../worldobject.h"

class BulletBody : public MapElement {
private:
	static std::string type;

public:
	BulletBody(int width, int height, float x, float y, float rotationCenterX, float rotationCenterY, float rotation = 0);
	virtual std::string GetType();
	static std::string GetClassType();
};

class Bullet : public WorldObject {
private:
	BulletBody *body;
	float movingSpeed;

public:
	Bullet(BulletBody *body, float movingSpeed);
	virtual void Update(float delta);
};

class GunBarrel;

class BulletFactory {
private:
	float bulletsSpeed;
	GunBarrel *barrel;

public:
	BulletFactory(GunBarrel *barrel, float bulletsSpeed);
	Bullet *Create();
};