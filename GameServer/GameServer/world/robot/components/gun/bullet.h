#pragma once

#include "../../../worldobject.h"

class BulletBody : public MapElement {
private:
	static std::string type;
	int damageInfluence;
	bool isDestroyed;

public:
	BulletBody(int damageInfluence, int width, int height, Point p, Point rotationCenter, float rotation = 0);
	int GetDamageInfluence();
	void Destroy();
	bool IsDestroyed();
	static std::string GetClassType();
};

class World;

class Bullet : public WorldObject {
private:
	World *world;
	BulletBody *body;
	float movingSpeed;

public:
	Bullet(BulletBody *body, World *world, float movingSpeed);
	virtual void Update(float delta);
};

class GunBarrel;

class BulletFactory {
private:
	float bulletsSpeed;
	int bulletsDamageInfluence;
	GunBarrel *barrel;
	World *world;

public:
	BulletFactory(GunBarrel *barrel, World *world, float bulletsSpeed, int bulletsDamageInfluence);
	Bullet *Create();
};