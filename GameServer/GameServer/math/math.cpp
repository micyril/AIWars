#include "math.h"
using namespace std;

Rectangle::Rectangle(int width, int height, Point p, float rotation) : 
	width(width) , height(height), vertice(p){
		rotationCenter = Point(vertice.x + width / 2,  vertice.y + height / 2);
		this->setAngle(rotation);
}


Rectangle::Rectangle(int width, int height, Point p, Point rotationCenter, float rotation) : 
	width(width) , height(height), vertice(p), rotationCenter(rotationCenter){
		this->setAngle(rotation);
}


void  Rectangle::setAngle(float angle){
	this->rotation.changeAngle(angle);
}
float  Rectangle::getAngle(){
	return  rotation.getAngle();
}
std::string Rectangle::serializeWithoutBrackets() {
	stringstream stream;
	stream <<	"\"position\": {\"x\": " << vertice.x << ", \"y\": " << vertice.y << "}," <<
				"\"width\": " << width << ", \"height\": " << height << "," <<
				"\"angle\": " << rotation.getAngle() << "," <<
				"\"rotationcenter\": {\"x\": " << rotationCenter.x << ", \"y\": " << rotationCenter.y << "}"; // TODO: записать сюда координаты центра поворота
	return stream.str();
}
std::vector<Point> Rectangle::getPoints(){
	std::vector<Point> rect(4);
	Point shift;
	shift.x = vertice.x;
	shift.y = vertice.y;
	rect[0] = (rotation*(shift - rotationCenter)) + rotationCenter;

	shift.x = vertice.x;
	shift.y = vertice.y + height;
	rect[1] = (rotation*(shift - rotationCenter)) + rotationCenter;

	shift.x = vertice.x + width;
	shift.y = vertice.y + height;
	rect[2] = (rotation*(shift - rotationCenter)) + rotationCenter;

	shift.x = vertice.x + width;
	shift.y = vertice.y;
	rect[3] = (rotation*(shift - rotationCenter)) + rotationCenter;
	return rect;
}
std::vector<Edge> Rectangle::getEdges(){
	std::vector<Edge> rect_edges(4);
	std::vector<Point> rect = getPoints();
	Point tmp = rect[0];
	
	for(int i = 0;i<4;i++){
		rect_edges[i].first = tmp;
		rect_edges[i].second = rect[(i+1) % 4];
		tmp = rect[(i+1) % 4];
	}
	return rect_edges;
}
std::string Rectangle::Serialize() {
	stringstream stream;
	stream << "{" << serializeWithoutBrackets() << "}";
	return stream.str();
}
BBox::BBox(){
	_min = Point(FLT_MAX, FLT_MAX);
	_max = Point(-FLT_MAX, -FLT_MAX);
}
BBox::BBox(std::vector<Point> rect){
	_min = Point(FLT_MAX, FLT_MAX);
	_max = Point(-FLT_MAX, -FLT_MAX);
	for(auto it =  rect.begin();it!= rect.end();it++){
		_min = minPoint(_min, *it);
		_max = maxPoint(_max, *it);
	}
		
}
Mat2::Mat2(){
	a00 = 0.0f;
	a01 = 0.0f;
	a10 = 0.0f;
	a11 = 0.0f;
	angle = 0.0f;
}
Mat2::Mat2(float rot){
		a00 = cos(rot);
		a01 = sin(rot);
		a10 = -sin(rot);
		a11 = cos(rot);
		angle = rot;
}
float Mat2::getAngle(){
	return angle;
}
void Mat2::changeAngle(float rot){
	a00 = cos(rot);
	a01 = sin(rot);
	a10 = -sin(rot);
	a11 = cos(rot);
	angle = rot;
}
float Point::norm(){
	return sqrt(this->x*this->x + this->y*this->y);
}
float Point::sqr_norm(){
	return (this->x*this->x + this->y*this->y);
}