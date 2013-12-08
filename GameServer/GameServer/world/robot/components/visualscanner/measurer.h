#pragma once

#include "../../../mapelement.h"

class Measurer {
public:
	static void FindDistanceAndDirectionToRectangle(Rectangle *rectangle, Point p, 
		float sectorBegin, float sectorEnd, float maxDistance,  float &distance, float &direction);
};