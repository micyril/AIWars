#pragma once

#include <exception>
#include <sstream>
#include "../mapelement.h"

class CollisionResolverNotFoundException : public std::exception {
private:
	std::string mapElementType1;
	std::string mapElementType2;

public:
	CollisionResolverNotFoundException(std::string mapElementType1, std::string mapElementType2) :
		mapElementType1(mapElementType1), mapElementType2(mapElementType2) {}

	virtual const char* what() const throw() {
		std::stringstream stream;
		stream << "Can't find CollisionResolver for " << mapElementType1 << " and " << mapElementType2 << " MapElement's types";
		return stream.str().c_str();
	}
};