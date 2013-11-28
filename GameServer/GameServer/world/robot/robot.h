#pragma once

#include <string>
#include <map>
#include "../worldobject.h"
#include "../mapelement.h"
#include "components/robotcomponent.h"
#include "task.h"

class Robot;

class RobotMapElement : public MapElement {
private:
	static std::string type;
	int health;

protected:
	Robot *robot;

public:
	RobotMapElement(int width, int height, float x = 0, float y = 0, int health = 100);
	void SetRobot(Robot *robot);
	Robot * const GetRobot();
	void Damage(int points);
	int GetHealth();
	virtual std::string GetType();
	static std::string GetClassType();
};

class RobotFrame : public RobotMapElement {
public:
	RobotFrame(int width, int height, float x = 0, float y = 0);
};

class Robot : public WorldObject {
private:
	//todo: use safe pointers
	std::map<std::string, RobotComponent*> commandToRobotComponent;
	Task *undoLastMovementTask;

public:
	Robot(RobotFrame *frame, std::list<RobotComponent*>& robotComponents);
	RobotFrame *frame;

	std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
	void SetUndoLastMovementTask(Task *undoLastMovementTask);
	void UndoLastMovement();
};