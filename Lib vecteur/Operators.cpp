#include "Operators.h"

/// <summary>
/// Adds a Point and a Direction.
/// Translates the given Point by the given Direction.
/// </summary>
/// <param name="p">The point to be translated.</param>
/// <param name="d">The direction vector used for translation.</param>
/// <returns>A new Point resulting from p + d.</returns>
Point operator+(const Point& p, const Direction& d)
{
    return Point(p.GetX() + d.GetX(), p.GetY() + d.GetY(), p.GetZ() + d.GetZ());
}

/// <summary>
/// Adds a Direction and a Point.
/// Allows commutative addition of Point and Direction (same as Point + Direction).
/// </summary>
/// <param name="d">The direction vector used for translation.</param>
/// <param name="p">The point to be translated.</param>
/// <returns>A new Point resulting from d + p.</returns>
Point operator+(const Direction& d, const Point& p)
{
    return Point(p.GetX() + d.GetX(), p.GetY() + d.GetY(), p.GetZ() + d.GetZ());
}

/// <summary>
/// Stream output operator for Point.
/// Prints the Point in the format (x, y, z).
/// </summary>
/// <param name="os">The output stream.</param>
/// <param name="p">The point to print.</param>
/// <returns>Reference to the output stream.</returns>
std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os << "(" << p.GetX() << ", " << p.GetY() << ", " << p.GetZ() << ")";
    return os;
}

/// <summary>
/// Stream output operator for Direction.
/// Prints the Direction in the format (x, y, z).
/// </summary>
/// <param name="os">The output stream.</param>
/// <param name="d">The direction vector to print.</param>
/// <returns>Reference to the output stream.</returns>
std::ostream& operator<<(std::ostream& os, const Direction& d)
{
    os << "(" << d.GetX() << ", " << d.GetY() << ", " << d.GetZ() << ")";
    return os;
}

/// <summary>
/// Multiplies a Direction by a scalar.
/// </summary>
/// <param name="scalar">The scalar value to multiply with.</param>
/// <param name="dir">The direction vector to be scaled.</param>
/// <returns>A new Direction scaled by the given value.</returns>
Direction operator*(double scalar, const Direction& dir)
{
    return Direction(dir.GetX() * scalar, dir.GetY() * scalar, dir.GetZ() * scalar);
}

/// <summary>
/// Multiplies a Direction by a scalar.
/// </summary>
/// <param name="dir">The direction vector to be scaled.</param>
/// <param name="scalar">The scalar value to multiply with.</param>
/// <returns>A new Direction scaled by the given value.</returns>
Direction operator*(const Direction& dir, double scalar)
{
	return Direction(dir.GetX() * scalar, dir.GetY() * scalar, dir.GetZ() * scalar);
}
