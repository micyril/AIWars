#pragma once
#include <string>
class Serializable {
public:
	// сериализыция в JSON строку
	virtual std::string Serialize()=0;
};

