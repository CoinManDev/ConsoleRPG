#pragma once

#include <iostream>
#include <vector>
#include "Point.h"

class Entity;

class Map
{
private:
	std::vector<std::vector<std::vector<Entity*>>> mEntityMap{};
	const int mWidth{};
	const int mHeight{};

	std::vector<Entity*>& getEntitySlot(const Point& position);
	void addEntity(const Point& position, Entity* entity);
	void removeEntity(const Point& position, Entity* entity);

	friend Entity;

public:
	Map(int width, int height);
	Map(const Map& other) = delete;
	Map& operator=(const Map& other) = delete;

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

	Entity* getEntityAtSlot(const std::vector<Entity*>& slot) const;
	Entity* getEntity(const Point& position);
	bool isPositionInBounds(const Point& position) const;

	friend std::ostream& operator<<(std::ostream& out, const Map& map);
};
