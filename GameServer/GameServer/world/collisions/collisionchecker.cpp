#include "collisionchecker.h"
#include "../../exceptions.h"
#include <algorithm>
using namespace std;
#define EPSILON 1e-9f


bool CollisionChecker::checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1){
	for(auto i = rect0.begin();i != rect0.end();i++)
		for(auto j = rect1.begin();j != rect1.end();j++)
			if(intersect2D_2Segments(*i,*j))
				return true;			
	return false;
}
bool CollisionChecker::checkPoints(std::vector<Point> &rect0, std::vector<Point> &rect1){
	BBox b0(rect0);
	BBox b1(rect1);
	bool check0 = belong(b0, b1);
	bool check1 = belong(b1, b0);
	return check0 || check1;
}
bool CollisionChecker::Check(Rectangle *rectangle1, Rectangle *rectangle2) {
	return checkEdges(rectangle1->getEdges(), rectangle2->getEdges()) ||
		checkPoints(rectangle1->getPoints(),rectangle2->getPoints());
}