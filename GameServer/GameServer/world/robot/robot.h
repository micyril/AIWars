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

	virtual std::string serializeWithoutBrackets();

public:
	
	RobotMapElement(std::string viewType, int width, int height, Point p = Point(0.0f,0.0f), int health = 100);
	virtual ~RobotMapElement() = 0 { };
	void SetRobot(Robot *robot);
	Robot * const GetRobot();
	void Damage(int points);
	int GetHealth();
	static std::string GetClassType();
};

class RobotFrame : public RobotMapElement {
public:
	
	RobotFrame(int width, int height, Point p = Point(0.0f,0.0f));
};

class Robot : public WorldObject {
private:
	std::map<std::string, RobotComponent*> commandToRobotComponent;
	std::list<RobotComponent *> robotComponents;
	Task *undoLastMovementTask;
	int id;
	static int lastId;

	void defineId();

public:
	Robot(RobotFrame *frame, std::list<RobotComponent*>& robotComponents);
	RobotFrame *frame;

	std::string Execute(const std::string &command, const std::string &arg);
	virtual void Update(float delta);
	void SetUndoLastMovementTask(Task *undoLastMovementTask);
	void UndoLastMovement();
	int GetId() { return id; }
};