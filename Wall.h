#pragma once

#include "Entity.h"

class Wall : public Entity
{
public:
	Wall(Map* map, const Point& position);
};
