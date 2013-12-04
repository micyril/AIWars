#pragma once

#include "../../../mapelement.h"

class Measurer {
public:
	void FindDistanceAndDirectionToRectangle(Rectangle *rectangle, float x, float y, 
		float sectorBegin, float sectorEnd, float max_distance,  float &distance, float &direction);
};