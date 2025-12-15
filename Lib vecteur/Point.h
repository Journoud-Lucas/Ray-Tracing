#pragma once

#include "Direction.h"

class Point
{
public:
	Point(double x = 0.0, double y = 0.0, double z = 0.0);

	void setX(double x) { m_x = x; };
	void setY(double y) { m_x = y; };
	void setZ(double z) { m_x = z; };

	double GetX() const { return m_x; };
	double GetY() const { return m_y; };
	double GetZ() const { return m_z; };

	void Translate(const Direction& dir);
	Point Translated(const Direction& dir) const;

	bool operator==(const Point& other) const;
	Point& operator+=(const Direction& other);
	Point& operator-=(const Direction& other);
	Direction operator-(const Point& other) const;

private: 
	double m_x, m_y, m_z;
};
