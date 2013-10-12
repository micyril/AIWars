#pragma once
#include <string>

class Serializable {
public:
	// сериализация в JSON строку
	virtual std::string Serialize()=0;
};
