#pragma once

#include "Entity.h"

int sign(int n);

class Block
{
private:
	std::vector<std::vector<std::shared_ptr<Entity>>> mBlock{}; // shared so that block can be used in vectors
	Point mStart{};

public:
	Block(const Entity& sample, const Point& start, const Point& end, const Point& gap = {});

	Entity& getEntity(const Point& position);
};
