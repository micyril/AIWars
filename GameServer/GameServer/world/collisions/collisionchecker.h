#pragma once

#include <vector>
#include "../../math/math.h"
#include "../mapelement.h"
#define EPSILON 1e-9f

class CollisionChecker {
	static bool checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1);
	static bool checkPoints(std::vector<Point> &rect0, std::vector<Point> &rect1);
public:
	static bool Check(Rectangle* rectangle1, Rectangle* rectangle2);
};
#undef EPSILON