#pragma once

#include "../../../mapelement.h"

class Measurer {
public:
	static void FindDistanceAndDirectionToRectangle(Rectangle *rectangle, float x, float y, 
		float sectorBegin, float sectorEnd, float maxDistance,  float &distance, float &direction);
};