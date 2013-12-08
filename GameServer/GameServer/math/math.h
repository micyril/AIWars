#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <sstream>
#define EPSILON 1e-9f
#include "../../../WebHandler/WebHandler/Serializable.h"



struct Point{
	float x;
	float y;
	Point(){}
	Point(float x1, float  y1) : x(x1) , y(y1) {}
	float norm();
	float sqr_norm();
};

struct Edge{
	Point first;
	Point second;
	Edge(){}
	Edge(Point p0, Point p1) : first(p0), second(p1){}
};
struct BBox{
	Point _min;
	Point _max;
	BBox();
	BBox(std::vector<Point> rect);
	BBox(Point min, Point max);
};
class Mat2{
	float angle;
public:
	float a00, a01, a10,a11;
	float getAngle();
	Mat2();
	Mat2(float rot);
	void changeAngle(float rot);
};

inline Point operator-(Point p0, Point p1){
	return Point(p0.x - p1.x, p0.y - p1.y);
}
inline Point operator-( Point p){
	return Point(0.0f, 0.0f) - p;
}
inline Point operator+(Point p0, Point p1){
	return Point(p0.x + p1.x, p0.y + p1.y);
}

inline Point operator+=(Point &p0, Point &p1){
	p0 = p0 + p1;
	return p0;
}
inline Point minPoint(Point p0, Point p1){
	return  Point(std::min<float>(p0.x, p1.x), std::min<float>(p0.y,p1.y));
}
inline Point maxPoint(Point p0, Point p1){
	return  Point(std::max<float>(p0.x, p1.x), std::max<float>(p0.y,p1.y));
}
inline bool operator<(Point &p0, Point &p1){
	return (p0.x < p1.x) && (p0.y < p1.y);
}
inline bool operator>=(Point &p0, Point &p1){
	return !(p0 < p1) ;
}
inline bool operator<=(Point &p0, Point &p1){
	return (p0.x <= p1.x) && (p0.y <= p1.y);
}
inline bool operator>(Point &p0, Point &p1){
	return !(p0 <= p1) ;
}
inline bool belong(BBox main, BBox checkig){
	return (main._min < checkig._min) && (checkig._max < main._max); 
}

inline Point operator*(float a, Point p){
		return Point(a*p.x, a*p.y);
}
inline bool operator==(Point p0, Point p1){
	return (fabs(p0.x - p1.x) < EPSILON) &&  (fabs(p0.y - p1.y) < EPSILON);
}
inline bool operator!=(Point p0, Point p1){
	return (fabs(p0.x - p1.x) > EPSILON) ||  (fabs(p0.y - p1.y) > EPSILON);
}
inline Point operator*(Mat2 m, Point p){
	return Point(m.a00*p.x + m.a01*p.y, m.a10*p.x + m.a11*p.y);
}


class Rectangle : public Serializable {


	Mat2 rotation;
public:
	int width;
	int height;
	Point vertice;
	Point rotationCenter;
	Rectangle(int width, int height, Point p, float rotation = 0);
	Rectangle(int width, int height, Point p, Point rotationCenter, float rotation = 0);
	float getAngle();
	void setAngle(float angle);
	Mat2 getRotation();
	void setRotation(Mat2 rot);
	virtual std::string Serialize();
	std::vector<Edge> getEdges();
	std::vector<Point> getPoints();
protected:
	virtual std::string serializeWithoutBrackets();
};


inline float dot(Point p0, Point p1){
	return p0.x*p1.x + p0.y*p1.y;
}

inline float  perp(Point p0, Point p1) { 
	return p0.x * p1.y - p0.y * p1.x;
}
inline float distanceToPoint(Point p0, Point p1){
	return sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y));
}

inline float distance_Point_to_Segment( Point P, Edge S, Point &I)
{
	Point v =  S.first;
	Point w = S.second;
	const float l2 = (w-v).sqr_norm();  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0){ 
		I = v;
		return distanceToPoint(P, v); 
	}  // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const float t = dot(P - v, w - v) / l2;
  if (t < 0.0){
	  I = v;
	  return distanceToPoint(P, v);
  }       // Beyond the 'v' end of the segment
  else if (t > 1.0){
	  I = w;
	  return distanceToPoint(P, w); } // Beyond the 'w' end of the segment
  const Point projection = v + t * (w - v);  // Projection falls on the segment
  I =  projection;
  return distanceToPoint(P, projection);
}
inline Point Segment_Crossing (Edge e1, Edge e2){
	float	dx1 = e1.second.x - e1.first.x;
	float	dy1 = e1.second.y - e1.first.y;
	float	dx2 = e2.second.x - e2.first.x;
	float	dy2 = e2.second.y - e2.first.y;
	float x = dy1 * dx2 - dy2 * dx1;
	
	float y = e2.first.x * e2.second.y - e2.first.y * e2.second.x;
	x = ((e1.first.x * e1.second.y - e1.first.y *  e1.second.x) * dx2 - y * dx1) / x;
	y = (dy2 * x - y) / dx2;
	return Point(x,y);
}
inline bool inSegment( Point P, Edge S)
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

inline bool intersect2D_2Segments( Edge S1, Edge S2 )
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