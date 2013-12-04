#include "math.h"

Mat2::Mat2(float rot){
		a00 = cos(rot);
		a01 = sin(rot);
		a10 = -sin(rot);
		a11 = cos(rot);
	}
float Point::norm(){
	return sqrt(this->x*this->x + this->y*this->y);
}
float Point::sqr_norm(){
	return (this->x*this->x + this->y*this->y);
}