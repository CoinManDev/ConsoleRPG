#include "DoorPair.h"

DoorPair::DoorPair(Map* firstMap, const Point& firstPosition, Map* secondMap, const Point& secondPosition)
	: first{ firstMap, firstPosition, this }, second{ secondMap, secondPosition, this }
{

}

DoorPair::DoorPair(const DoorPair& other)
	: first{ other.first.getMap(), other.first.getPosition(), this }, second{ other.second.getMap(), other.second.getPosition(), this }
{

}
