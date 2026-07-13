#pragma once

#include <iostream>

struct Point
{
	int x{};
	int y{};
};

namespace Directions
{
	inline const Point left{ -1, 0 };
	inline const Point right{ 1, 0 };
	inline const Point up{ 0, -1 };
	inline const Point down{ 0, 1 };
}

std::ostream& operator<<(std::ostream& out, const Point& point);
Point operator+(const Point& p1, const Point& p2);
Point operator-(const Point& p1, const Point& p2);
bool operator==(const Point& p1, const Point& p2);
bool operator!=(const Point& p1, const Point& p2);
Point distance(const Point& p1, const Point& p2);
Point distanceAbs(const Point& p1, const Point& p2);
