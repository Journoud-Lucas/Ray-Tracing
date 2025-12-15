#pragma once

#include <cmath>
#include <utility>

class Direction
{
public:
	Direction(double x = 0.0, double y = 0.0, double z = 0.0);

	void setX(double x) { m_x = x; };
	void setY(double y) { m_y = y; };
	void setZ(double z) { m_z = z; };

	double GetX() const { return m_x; };
	double GetY() const { return m_y; };
	double GetZ() const { return m_z; };

	double Dot(const Direction& other) const;
	double Length() const;
	Direction Normalized() const;
	void Normalize();
	static std::pair<Direction, Direction> MakeBase(const Direction& normal);
	static Direction RotateVector(const Direction& normal, const Direction& localVec);

	bool operator==(const Direction& other) const;
	Direction operator+(const Direction& other) const;
	Direction operator-(const Direction& other) const;
	Direction& operator+=(const Direction& other);
	Direction& operator-=(const Direction& other);
	Direction operator-() const { return Direction(-m_x, -m_y, -m_z); }

private:
	double m_x, m_y, m_z;
};
