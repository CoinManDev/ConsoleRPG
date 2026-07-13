#include "Door.h"

Door::Door(Map* map, const Point& position, const DoorPair* doorPair)
	: Entity{ map, position, '*', Aesthetics::blue }, mDoorPair{ doorPair }
{

}
