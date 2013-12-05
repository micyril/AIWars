#pragma once

#include <vector>
#include "../../math/math.h"
#include "../mapelement.h"
#define EPSILON 1e-9f

class CollisionChecker {
	static bool checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1);
public:
	static bool Check(Rectangle* rectangle1, Rectangle* rectangle2);
};
#undef EPSILON