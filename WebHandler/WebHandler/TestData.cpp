#include "TestData.h"

TestData::TestData() {
	k = 10;
	s = "qwerty ololo";
	for (int i=0; i<5; i++)
		arr[i] = i;
}

std::string TestData::Serialize() {
	std::stringstream ss;
	ss << "{";
	ss << "\"k\": " << k;
	ss << ",";
	ss << "\"s\": " << "\"" << s << "\"";
	ss << ",";
	ss << "\"arr\": [";
	for (int i = 0; i<4; i++)
		ss << arr[i] << ",";
	ss << arr[4] << "]";
	ss << "}";
	return ss.str();
}