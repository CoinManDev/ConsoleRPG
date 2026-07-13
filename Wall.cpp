#include "Wall.h"

Wall::Wall(Map* map, const Point& position)
	: Entity{ map, position, '#', Aesthetics::magenta }
{

}
