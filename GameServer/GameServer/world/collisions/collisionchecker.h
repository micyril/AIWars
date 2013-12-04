#pragma once

#include "../mapelement.h"
#include <vector>
#include "../../math/math.h"
#define EPSILON 1e-9f

class CollisionChecker {
	static bool checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1);
public:
	static bool Check(MapElement *mapElement1, MapElement *mapElement2);
};
#undef EPSILON