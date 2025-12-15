#include "Point.h"

/// <summary>
/// Constructor for Point.
/// Initializes the point with the given coordinates.
/// </summary>
/// <param name="x">X coordinate.</param>
/// <param name="y">Y coordinate.</param>
/// <param name="z">Z coordinate.</param>
Point::Point(double x, double y, double z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/// <summary>
/// Translates the point by the given direction vector.
/// Modifies the current point.
/// </summary>
/// <param name="dir">The direction vector to translate by.</param>
void Point::Translate(const Direction& dir)
{
    m_x += dir.GetX();
    m_y += dir.GetY();
    m_z += dir.GetZ();
}

/// <summary>
/// Returns a new point translated by the given direction vector.
/// Does not modify the current point.
/// </summary>
/// <param name="dir">The direction vector to translate by.</param>
/// <returns>A new Point object after translation.</returns>
Point Point::Translated(const Direction& dir) const
{
    return Point(m_x + dir.GetX(), m_y + dir.GetY(), m_z + dir.GetZ());
}

/// <summary>
/// Equality operator for points.
/// Checks if two points have the same coordinates.
/// </summary>
/// <param name="other">The point to compare with.</param>
/// <returns>True if points are equal, false otherwise.</returns>
bool Point::operator==(const Point& other) const
{
    return m_x == other.GetX() && m_y == other.GetY() && m_z == other.GetZ();
}

/// <summary>
/// Adds a direction vector to the point.
/// Modifies the current point.
/// </summary>
/// <param name="other">The direction vector to add.</param>
/// <returns>Reference to the modified point.</returns>
Point& Point::operator+=(const Direction& other)
{
    m_x += other.GetX();
    m_y += other.GetY();
    m_z += other.GetZ();

    return *this;
}

/// <summary>
/// Subtracts a direction vector from the point.
/// Modifies the current point.
/// </summary>
/// <param name="other">The direction vector to subtract.</param>
/// <returns>Reference to the modified point.</returns>
Point& Point::operator-=(const Direction& other)
{
    m_x -= other.GetX();
    m_y -= other.GetY();
    m_z -= other.GetZ();

    return *this;
}

/// <summary>
/// Subtracts another point from this point.
/// Returns the direction vector from the other point to this point.
/// </summary>
/// <param name="other">The point to subtract.</param>
/// <returns>A Direction vector representing the difference.</returns>
Direction Point::operator-(const Point& other) const
{
	return Direction(m_x - other.GetX(), m_y - other.GetY(), m_z - other.GetZ());
}
