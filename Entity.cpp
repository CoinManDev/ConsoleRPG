#include "Entity.h"

void Entity::removeFromMap()
{
	mMap->removeEntity(mPosition, this);
}

Entity::Entity(Map* map, const Point& position, char symbol, Aesthetics::Color color)
	: mMap{ map }, mPosition{ position }, mSymbol{ symbol }, mColor{ color }
{
	mMap->addEntity(mPosition, this);
}

Entity::Entity(const Entity& other)
	: Entity{ other.mMap, other.mPosition, other.mSymbol, other.mColor }
{

}

Entity::~Entity()
{
	removeFromMap();
}

void Entity::setMap(Map* map)
{
	removeFromMap();
	map->addEntity(mPosition, this);
	mMap = map;
}

void Entity::moveTo(const Point& to)
{
	if (!mMap->isPositionInBounds(to))
		return;

	removeFromMap(); // remove first to account for the scenario when mPosition and to is same ( if we added first removeEntity would remove the instance just added as well )
	mMap->addEntity(to, this);
	mPosition = to;
}

void Entity::move(const Point& to)
{
	moveTo(mPosition + to);
}

std::ostream& operator<<(std::ostream& out, const Entity& entity)
{
	return out << entity.mColor << Aesthetics::bgColor << entity.mSymbol;
}

// Functions for map

Entity* Map::getEntityAtSlot(const std::vector<Entity*>& slot) const
{
	for (auto* entity : slot)
		if (entity->isActive())
			return entity;

	return nullptr;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
	for (const auto& row : map.mEntityMap)
	{
		for (const auto& slot : row)
		{
			Entity* entity{ map.getEntityAtSlot(slot) };
			if (entity)
				out << *entity << ' ' << Aesthetics::reset_color;
			else
				out << Aesthetics::bgColor << "  " << Aesthetics::reset_color;
		}

		out << '\n';
	}

	return out;
}
