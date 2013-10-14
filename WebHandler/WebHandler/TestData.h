#pragma once
#include "serializable.h"
#include <string>
#include <sstream>


class TestData : public Serializable {
public:
	int k;
	std::string s;
	int arr[5];
	
	TestData();
	virtual std::string Serialize();
};

