#include "Creature.h"

Creature::Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, int health, int damage)
	: Entity{ map, position, symbol, color }, CreatureBase{ health, damage }
{

}

Creature::Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, CreatureBase base)
	: Entity{ map, position, symbol, color }, CreatureBase{ base }
{

}
