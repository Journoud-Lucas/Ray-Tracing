#include "Direction.h"
#include "Operators.h"

/// <summary>
/// Constructor for Direction.
/// Initializes the direction vector with the given components.
/// </summary>
/// <param name="x">X component.</param>
/// <param name="y">Y component.</param>
/// <param name="z">Z component.</param>
Direction::Direction(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

/// <summary>
/// Computes the dot product with another direction.
/// </summary>
/// <param name="other">The other direction vector.</param>
/// <returns>The dot product as a double.</returns>
double Direction::Dot(const Direction& other) const
{
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

/// <summary>
/// Computes the length (magnitude) of the direction vector.
/// </summary>
/// <returns>The length as a double.</returns>
double Direction::Length() const
{
    return sqrt(Dot(*this));
}

/// <summary>
/// Returns a normalized (unit length) version of the direction vector.
/// Does not modify the current vector.
/// Return null vector, if length == 0
/// </summary>
/// <returns>A new Direction vector of length 1.</returns>
Direction Direction::Normalized() const
{
    double len = Length();
    if (len == 0) return Direction(0, 0, 0);
    return Direction(m_x / len, m_y / len, m_z / len);
}

/// <summary>
/// Normalizes the current direction vector in place.
/// </summary>
void Direction::Normalize()
{
    double len = Length();
    if (len == 0) return;
    m_x /= len;
    m_y /= len;
    m_z /= len;
}

/// <summary>
/// Equality operator for directions.
/// Checks if two direction vectors have the same components.
/// </summary>
/// <param name="other">The direction to compare with.</param>
/// <returns>True if directions are equal, false otherwise.</returns>
bool Direction::operator==(const Direction& other) const
{
    return m_x == other.GetX() && m_y == other.GetY() && m_z == other.GetZ();
}

/// <summary>
/// Adds two direction vectors and returns the result.
/// </summary>
/// <param name="other">The direction to add.</param>
/// <returns>A new Direction resulting from the addition.</returns>
Direction Direction::operator+(const Direction& other) const
{
    return Direction
    (
        m_x + other.GetX(),
        m_y + other.GetY(),
        m_z + other.GetZ()
    );
}

/// <summary>
/// Subtracts another direction vector from this one and returns the result.
/// </summary>
/// <param name="other">The direction to subtract.</param>
/// <returns>A new Direction resulting from the subtraction.</returns>
Direction Direction::operator-(const Direction& other) const
{
    return Direction
    (
        m_x - other.GetX(),
        m_y - other.GetY(),
        m_z - other.GetZ()
    );
}

/// <summary>
/// Adds another direction vector to this one in place.
/// </summary>
/// <param name="other">The direction to add.</param>
/// <returns>Reference to the modified direction vector.</returns>
Direction& Direction::operator+=(const Direction& other)
{
    m_x += other.GetX();
    m_y += other.GetY();
    m_z += other.GetZ();

    return *this;
}

/// <summary>
/// Subtracts another direction vector from this one in place.
/// </summary>
/// <param name="other">The direction to subtract.</param>
/// <returns>Reference to the modified direction vector.</returns>
Direction& Direction::operator-=(const Direction& other)
{
    m_x -= other.GetX();
    m_y -= other.GetY();
    m_z -= other.GetZ();

    return *this;
}

/// <summary>
/// Generates an orthonormal basis given a normal vector n.
/// </summary>
/// <param name="n">The normal vector.</param>
/// <returns>A pair of Direction vectors forming the basis.</returns>
std::pair<Direction, Direction> Direction::MakeBase(const Direction& normal)
{
    double x = normal.GetX();
    double y = normal.GetY();
    double z = normal.GetZ();

    double sign;
    if (z >= 0.0)
    {
       sign = 1.0;
    }
    else
    {
		sign = -1.0;
	}
    double a = -1.0 / (sign + z);
    double b = x * y * a;

    Direction baseX(1.0 + sign * x * x * a, sign * b, -sign * x);
    Direction baseY(b, sign + y * y * a, -y);

    return { baseX.Normalized(), baseY.Normalized() };
}

/// <summary>
/// Rotates a local vector to align with the given normal vector.
/// </summary>
/// <param name="normal">The normal vector to align with.</param>
/// <param name="localVec">The local vector to rotate.</param>
/// <returns>The rotated Direction vector.</returns>
Direction Direction::RotateVector(const Direction& normal, const Direction& localVec)
{
    auto [baseX, baseY] = Direction::MakeBase(normal);

    double x = localVec.GetX();
    double y = localVec.GetY();
    double z = localVec.GetZ();

    Direction rotated = x * baseX + y * baseY + z * normal;
    return rotated.Normalized();
}
