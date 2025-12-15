#include "Ray.h"

/// Constructor for Ray.
/// Initializes the ray with an origin point and a direction vector.
/// <param name="origin">The origin point of the ray.</param>
/// <param name="direction">The direction vector of the ray.</param>
Ray::Ray(const Point& origin, const Direction& direction)
{
	m_origin = origin;
	m_direction = direction.Normalized();
}
