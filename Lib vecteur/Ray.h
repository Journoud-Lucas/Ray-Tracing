#pragma once

#include "Point.h"

class Ray
{
private:
    Point m_origin;
    Direction m_direction;

public:
    Ray(const Point& origin, const Direction& direction);

	Point GetOrigin() const { return m_origin; }
	Direction GetDirection() const { return m_direction; }
};
