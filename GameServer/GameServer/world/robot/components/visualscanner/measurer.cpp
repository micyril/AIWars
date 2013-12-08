#include "measurer.h"
#include <vector>
#include "../../../../math/math.h"





/*
* Arguments
* Rectangle - rectangle for checking distance to it
* x,y - coordinates of center circle segment
* sectorBegin,  sectorEnd - is limint of circle segment
* distance, direction - return values
*/

void Measurer::FindDistanceAndDirectionToRectangle(Rectangle *rectangle, Point scan_center, 
			float sectorBegin, float sectorEnd, float max_distance, float &distance, float &direction){
	std::vector<Edge> rect_edges = rectangle->getEdges();

	float min_dist_to_vert = std::numeric_limits<float>::max();
	Point closest_I;
	Edge closest_E;
	for(int i = 0;i<rect_edges.size();i++){
		Point I;
		float tmp_dist = distance_Point_to_Segment(scan_center, rect_edges[i], I);
		if( tmp_dist < min_dist_to_vert){
			closest_E =  rect_edges[i];
			closest_I = I;
			min_dist_to_vert = tmp_dist;
		}
	}
	Point edge1 = closest_I - scan_center;
	Point edge2 = Point(scan_center.x + max_distance, scan_center.y) - scan_center;
	float angle = acos(dot(edge1, edge2)/(edge1.norm() * edge2.norm()));
	if(sectorBegin < angle && angle < sectorEnd ){
		distance = min_dist_to_vert;
		direction = angle;
		return;
	}
	if(sectorBegin > angle){
		Mat2 rot(sectorBegin);
		Point p = Segment_Crossing(closest_E, Edge(scan_center, (rot*(Point(scan_center.x + max_distance, scan_center.y)-scan_center)+scan_center)));
		Point I;
		distance = distanceToPoint(scan_center, p);
		direction = sectorBegin;
		return;
	}
	if(angle > sectorEnd ){
		Mat2 rot(sectorEnd);
		Point p = Segment_Crossing(closest_E, Edge(scan_center,(rot*(Point(scan_center.x + max_distance, scan_center.y)-scan_center)+scan_center)));
		Point I;
		distance = distanceToPoint(scan_center, p);
		direction = sectorEnd;
		return;
	}
}