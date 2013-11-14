#include "collisionchecker.h"
#include "../../exceptions.h"
using namespace std;
#define EPSILON 1e-9f
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)

Mat2::Mat2(float rot){
		a00 = cos(rot);
		a01 = -sin(rot);
		a10 = sin(rot);
		a11 = cos(rot);
	}
Point operator-(Point p0, Point p1){
	return Point(p0.x - p1.x, p0.y - p1.y);
}

Point operator+(Point p0, Point p1){
	return Point(p0.x + p1.x, p0.y + p1.y);
}
Point operator*(float a, Point p){
		return Point(a*p.x, a*p.y);
}
bool operator==(Point p0, Point p1){
	return (fabs(p0.x - p1.x) < EPSILON) &&  (fabs(p0.y - p1.y) < EPSILON);
}
bool operator!=(Point p0, Point p1){
	return (fabs(p0.x - p1.x) > EPSILON) ||  (fabs(p0.y - p1.y) > EPSILON);
}
Point operator*(Mat2 m, Point p){
	return Point(m.a00*p.x + m.a01*p.y, m.a10*p.x + m.a11*p.y);
}
bool inSegment( Point P, Edge S)
{
    if (S.first.x != S.second.x) {    // S is not  vertical
        if (S.first.x <= P.x && P.x <= S.second.x)
            return 1;
        if (S.first.x >= P.x && P.x >= S.second.x)
            return 1;
    }
    else {    // S is vertical, so test y  coordinate
        if (S.first.y <= P.y && P.y <= S.second.y)
            return 1;
        if (S.first.y >= P.y && P.y >= S.second.y)
            return 1;
    }
    return 0;
}

bool intersect2D_2Segments( Edge S1, Edge S2 )
{
    Point    u = S1.second - S1.first;
    Point    v = S2.second - S2.first;
    Point    w = S1.first - S2.first;
    float     D = perp(u,v);

    if (fabs(D) < EPSILON) {           // S1 and S2 are parallel
        if (perp(u,w) != 0 || perp(v,w) != 0)  {
            return 0;                    // they are NOT collinear
        }
        // they are collinear or degenerate
        // check if they are degenerate  points
        float du = dot(u,u);
        float dv = dot(v,v);
        if (du==0 && dv==0) {            // both segments are points
            if (S1.first !=  S2.first)         // they are distinct  points
                 return 0;
            return 1;
        }
        if (du==0) {                     // S1 is a single point
            if  (inSegment(S1.first, S2) == 0)  // but is not in S2
                 return 0;
            return 1;
        }
        if (dv==0) {                     // S2 a single point
            if  (inSegment(S2.first, S1) == 0)  // but is not in S1
                 return 0;
            return 1;
        }
        // they are collinear segments - get  overlap (or not)
        float t0, t1;                    // endpoints of S1 in eqn for S2
        Point w2 = S1.second - S2.first;
        if (v.x != 0) {
                 t0 = w.x / v.x;
                 t1 = w2.x / v.x;
        }
        else {
                 t0 = w.y / v.y;
                 t1 = w2.y / v.y;
        }
        if (t0 > t1) {                   // must have t0 smaller than t1
                 float t=t0; t0=t1; t1=t;    // swap if not
        }
        if (t0 > 1 || t1 < 0) {
            return 0;      // NO overlap
        }
        t0 = t0<0? 0 : t0;               // clip to min 0
        t1 = t1>1? 1 : t1;               // clip to max 1
        if (t0 == t1)                // intersect is a point
            return 1;
        return 1;
    }
    float     sI = perp(v,w) / D;
    if (sI < 0 || sI > 1)                // no intersect with S1
        return 0;
    // get the intersect parameter for S2
    float     tI = perp(u,w) / D;
    if (tI < 0 || tI > 1)                // no intersect with S2
        return 0;
    return 1;
}
bool CollisionChecker::checkEdges(std::vector<Edge> &rect0, std::vector<Edge> &rect1){
	for(auto i = rect0.begin();i != rect0.end();i++)
		for(auto j = rect1.begin();j != rect1.end();j++)
			if(intersect2D_2Segments(*i,*j))
				return true;			
	return false;
}

bool CollisionChecker::Check(MapElement *mapElement0, MapElement *mapElement1) {
	Point a(1,1);
	Point b(1,1);
	Point c = a + b;
	Mat2 m0(mapElement0->rotation);
	std::vector<Point> rect0(4);
	Point center(mapElement0->rotationCenterX, mapElement0->rotationCenterY);//должно быть в mapElement
	Point shift;
	shift.x = -mapElement0->width*0.5f;
	shift.y = -mapElement0->height*0.5f;
	rect0[0] = center + (m0*shift);

	shift.x = -mapElement0->width*0.5f;
	shift.y = +mapElement0->height*0.5f;
	rect0[1] = center + (m0*shift);

	shift.x = +mapElement0->width*0.5f;
	shift.y = +mapElement0->height*0.5f;
	rect0[2] = center + (m0*shift);

	shift.x = +mapElement0->width*0.5f;
	shift.y = -mapElement0->height*0.5f;
	rect0[3] = center + (m0*shift);


	Mat2 m1(mapElement1->rotation);
	std::vector<Point> rect1(4);
	center = Point(mapElement1->rotationCenterX, mapElement1->rotationCenterY);//должно быть в mapElement
	shift.x = -mapElement1->width*0.5f;
	shift.y = -mapElement1->height*0.5f;
	rect1[0] = center + (m1*shift);

	shift.x = -mapElement1->width*0.5f;
	shift.y = +mapElement1->height*0.5f;
	rect1[1] = center + (m1*shift);

	shift.x = +mapElement1->width*0.5f;
	shift.y = +mapElement1->height*0.5f;
	rect1[2] = center + (m1*shift);

	shift.x = +mapElement1->width*0.5f;
	shift.y = -mapElement1->height*0.5f;
	rect1[3] = center + (m1*shift);

	std::vector<Edge> rect0_edges(4);
	std::vector<Edge> rect1_edges(4);
	Point tmp = rect0[0];
	
	for(int i = 0;i<4;i++){
		rect0_edges[i].first = tmp;
		rect0_edges[i].second = rect0[(i+1) % 4];
		tmp = rect0[(i+1) % 4];
	}
	tmp = rect1[0];
	for(int i = 0;i<4;i++){
		rect1_edges[i].first = tmp;
		rect1_edges[i].second = rect1[(i+1) % 4];
		tmp = rect1[(i+1) % 4];
	}

	return checkEdges(rect0_edges, rect1_edges);
}