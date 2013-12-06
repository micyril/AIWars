#include "measurer.h"
#include <vector>
#include "../../../../math/math.h"
//ToDo переместить эту тупость в math и заменить на функции
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)
#define d(u, v)	   (sqrt(((u).x - (v).x)*((u).x - (v).x) + ((u).y - (v).y)*((u).y - (v).y)))
float distance_Point_to_Segment( Point P, Edge S, Point &I)
{
	Point v =  S.first;
	Point w = S.second;
	const float l2 = (w-v).sqr_norm();  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0){ 
		I = v;
		return d(P, v); 
	}  // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const float t = dot(P - v, w - v) / l2;
  if (t < 0.0){
	  I = v;
	  return d(P, v);
  }       // Beyond the 'v' end of the segment
  else if (t > 1.0){
	  I = w;
	  return d(P, w); } // Beyond the 'w' end of the segment
  const Point projection = v + t * (w - v);  // Projection falls on the segment
  I =  projection;
  return d(P, projection);
}
Point Segment_Crossing (Edge e1, Edge e2){
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




/*
* Arguments
* Rectangle - rectangle for checking distance to it
* x,y - coordinates of center circle segment
* sectorBegin,  sectorEnd - is limint of circle segment
* distance, direction - return values
*/

void Measurer::FindDistanceAndDirectionToRectangle(Rectangle *rectangle, float x, float y, 
			float sectorBegin, float sectorEnd, float max_distance, float &distance, float &direction){
	Mat2 m0(rectangle->rotation);
	std::vector<Point> rect0(4);
	Point center(rectangle->rotationCenterX, rectangle->rotationCenterY);//должно быть в mapElement
	Point shift;
	shift.x = rectangle->x;
	shift.y = rectangle->y;
	rect0[0] = (m0*(shift - center)) + center;

	shift.x = rectangle->x;
	shift.y = rectangle->y + rectangle->height;
	rect0[1] = (m0*(shift - center)) + center;

	shift.x = rectangle->x + rectangle->width;
	shift.y = rectangle->y + rectangle->height;
	rect0[2] = (m0*(shift - center)) + center;

	shift.x = rectangle->x + rectangle->width;
	shift.y = rectangle->y;
	rect0[3] = (m0*(shift - center)) + center;

	std::vector<Edge> rect0_edges(4);
	Point tmp = rect0[0];
	for(int i = 0;i<4;i++){
		rect0_edges[i].first = tmp;
		rect0_edges[i].second = rect0[(i+1) % 4];
		tmp = rect0[(i+1) % 4];
	}
	float min_dist_to_vert = std::numeric_limits<float>::max();
	Point scan_center(x,y);
	Point closest_I;
	Edge closest_E;
	for(int i = 0;i<rect0_edges.size();i++){
		Point I;
		float tmp_dist = distance_Point_to_Segment(scan_center, rect0_edges[i], I);
		if( tmp_dist < min_dist_to_vert){
			closest_E =  rect0_edges[i];
			closest_I = I;
			min_dist_to_vert = tmp_dist;
		}
	}
	Point edge1 = closest_I - scan_center;
	Point edge2 = Point(x + max_distance, y) - scan_center;
	float angle = acos(dot(edge1, edge2)/(edge1.norm() * edge2.norm()));
	if(sectorBegin < angle && angle < sectorEnd ){
		distance = min_dist_to_vert;
		direction = angle;
		return;
	}
	if(sectorBegin > angle){
		Mat2 rot(sectorBegin);
		Point p = Segment_Crossing(closest_E, Edge(scan_center, (rot*(Point(x + max_distance, y)-scan_center)+scan_center)));
		Point I;
		distance = d(scan_center, p);
		direction = sectorBegin;
		return;
	}
	if(angle > sectorEnd ){
		Mat2 rot(sectorEnd);
		Point p = Segment_Crossing(closest_E, Edge(scan_center,(rot*(Point(x - max_distance, y)-scan_center)+scan_center)));
		Point I;
		distance = d(scan_center, p);
		direction = sectorEnd;
		return;
	}
}