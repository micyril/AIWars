#pragma once

class WorldObject {
public:
	virtual ~WorldObject() {}
	virtual void Update(float delta) = 0;
};