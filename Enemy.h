#pragma once

#include <array>
#include <string>
#include "Creature.h"

using namespace std::string_literals;

class Enemy : public Creature
{
public:
	enum Type
	{
		goblin,
		exile,
		knight,
		ogre,

		max_types,
	};

	inline static const std::array names{ "goblin"s, "exile"s, "knight"s, "ogre"s };
	static_assert(names.size() == max_types);

	constexpr static std::array symbols{ 'G', 'E', 'K', 'O' };
	static_assert(symbols.size() == max_types);

	inline static const std::array stats{ CreatureBase{ 5, 1 }, CreatureBase{ 10, 2 }, CreatureBase{ 20, 4 }, CreatureBase{ 40, 8 } };
	static_assert(stats.size() == max_types);

	constexpr static std::array tiers{ 1, 2, 3, 4 };
	static_assert(tiers.size() == max_types);

	constexpr static int baseXp{ 10 };

private:
	Type mType{};
	std::string mName{};

public:
	Enemy(Map* map, const Point& position, Type type);

	const std::string& getName() const { return mName; }
	int getTier() const { return tiers[mType]; }
	int getXp() const { return baseXp * getTier(); }
};
