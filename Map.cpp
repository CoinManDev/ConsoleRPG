#include <cassert>
#include "Map.h"

std::vector<Entity*>& Map::getEntitySlot(const Point& position)
{
	return mEntityMap[static_cast<std::size_t>(position.y)][static_cast<std::size_t>(position.x)];
}

void Map::addEntity(const Point& position, Entity* entity)
{
	getEntitySlot(position).push_back(entity);
}

void Map::removeEntity(const Point& position, Entity* entity)
{
	std::erase(getEntitySlot(position), entity); // removes ALL instances of entity
}

Map::Map(int width, int height)
	: mWidth{ width }, mHeight{ height }
{
	assert((width >= 0 && height >= 0) && "Map dimensions smaller than zero");

	mEntityMap.resize(static_cast<std::size_t>(height));
	for (auto& row : mEntityMap)
		row.resize(static_cast<std::size_t>(width));
}

Entity* Map::getEntity(const Point& position)
{
	if (!isPositionInBounds(position))
		return nullptr;

	return getEntityAtSlot(getEntitySlot(position));
}

bool Map::isPositionInBounds(const Point& position) const
{
	return (position.x >= 0 && position.x < mWidth) &&
		(position.y >= 0 && position.y < mHeight);
}
