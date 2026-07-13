#include "Point.h"

#include <cmath>

std::ostream& operator<<(std::ostream& out, const Point& point)
{
	return out << "x: " << point.x << ", y: " << point.y;
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}

bool operator==(const Point& p1, const Point& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

bool operator!=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
}

Point distance(const Point& p1, const Point& p2)
{
	return p1 - p2;
}

Point distanceAbs(const Point& p1, const Point& p2)
{
	return Point{ std::abs(p1.x - p2.x), std::abs(p1.y - p2.y) };
}
