#pragma once
#include <string>
class Serializable {
public:
	// ������������ � JSON ������
	virtual std::string Serialize()=0;
};

