#pragma once

#include "Entity.h"
#include "CreatureBase.h"

class Creature : public Entity, public CreatureBase
{
public:
	Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, int health, int damage);
	Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, CreatureBase base);
};
