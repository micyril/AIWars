#include <math.h>
#include <vector>
#define EPSILON 1e-9f
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
struct Mat2{
	float a00, a01, a10,a11;
	Mat2(float rot);
};
inline Point operator-(Point p0, Point p1){
	return Point(p0.x - p1.x, p0.y - p1.y);
}

inline Point operator+(Point p0, Point p1){
	return Point(p0.x + p1.x, p0.y + p1.y);
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