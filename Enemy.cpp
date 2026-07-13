#include "Enemy.h"

Enemy::Enemy(Map* map, const Point& position, Type type)
	: mType{ type }, mName{ names[type] }, Creature{ map, position, symbols[type], Aesthetics::red, stats[type] }
{

}
