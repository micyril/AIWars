#pragma once

#include "../mapelement.h"
#include <vector>
#define EPSILON 1e-9f
struct Point{
	float x;
	float y;
	Point(){}
	Point(float x1, float  y1) : x(x1) , y(y1) {}
	
};

struct Edge{
	Point first;
	Point second;
	Edge(){}
	Edge(Point p0, Point p1) : first(p0), second(p1){}
};
struct Mat2{
	float a00, a01, a10,a11;
	Mat2(float rot);
};

class CollisionChecker {
	static bool checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1);
public:
	static bool Check(MapElement *mapElement1, MapElement *mapElement2);
};
#undef EPSILON