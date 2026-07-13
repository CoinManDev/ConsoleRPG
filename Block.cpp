#include "Block.h"

#include <cassert>

int sign(int n)
{
	if (n == 0)
		return 0;

	return (n / std::abs(n));
}

Block::Block(const Entity& sample, const Point& start, const Point& end, const Point& gap)
	: mStart{ start }
{
	Point distAbs{ distanceAbs(start, end) };
	mBlock.resize(static_cast<std::size_t>(distAbs.y + 1));
	for (auto& row : mBlock)
		row.resize(static_cast<std::size_t>(distAbs.x + 1));

	Point dist{ distance(start, end) };
	for (int y{}; y <= distAbs.y; y += (1 + gap.y))
	{
		for (int x{}; x <= distAbs.x; x += (1 + gap.x))
		{
			auto& ptr{ mBlock[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] };
			ptr = std::make_shared<Entity>(sample);
			ptr->moveTo(start - Point{ x * sign(dist.x), y * sign(dist.y) });
		}
	}
}

Entity& Block::getEntity(const Point& position)
{
	Point distAbs{ distanceAbs(position, mStart) };
	auto& ptr{ mBlock[static_cast<std::size_t>(distAbs.y)][static_cast<std::size_t>(distAbs.x)] };
	assert(ptr && "Couldn't find entity in block");
	assert(ptr->getPosition() == position && "Faulty position in getEntity");

	return *ptr;
}
