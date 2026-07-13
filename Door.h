#pragma once

#include "Entity.h"

class DoorPair;

class Door : public Entity
{
private:
	const DoorPair* mDoorPair;

public:
	Door(Map* map, const Point& position, const DoorPair* doorPair);

	const DoorPair* getDoorPair() const { return mDoorPair; }
};
