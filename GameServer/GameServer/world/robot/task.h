#pragma once

class Task {
public:
	virtual ~Task() {}
	virtual void Perform() = 0;
};