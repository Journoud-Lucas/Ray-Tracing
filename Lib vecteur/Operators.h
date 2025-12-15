#pragma once

#include <iostream>
#include "Point.h"
#include "Direction.h"

Point operator+(const Point& p, const Direction& d);
Point operator+(const Direction& d, const Point& p);
Direction operator*(double scalar, const Direction& dir);
Direction operator*(const Direction& dir, double scalar);

std::ostream& operator<<(std::ostream& os, const Point& p);
std::ostream& operator<<(std::ostream& os, const Direction& d);
