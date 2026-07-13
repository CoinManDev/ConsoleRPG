#pragma once

#include "Door.h"

class DoorPair
{
private:
	Door first;
	Door second;

public:
	DoorPair(Map* firstMap, const Point& firstPosition, Map* secondMap, const Point& secondPosition);
	DoorPair(const DoorPair& other);

	const Door& getFirst() const { return first; }
	const Door& getSecond() const { return second; }
};
