#pragma once

#include <array>
#include <iostream>
#include "Item.h"

class Food : public Item
{
public:
	enum Type
	{
		bread,
		bread_two,
		beef,
		sandvich,

		max_types,
	};

	inline static constexpr std::array names{ "bread", "bread two", "beef", "sandvich" };
	static_assert(names.size() == max_types);

	inline static constexpr std::array descriptions{
		"Yummy bread.",
		"Yummier bread.",
		"Organic flesh.",
		"A sandwich with an odd choice of ingredients.",
	};
	static_assert(descriptions.size() == max_types);

	inline static const std::array heals{
		Effect::modifiedHealth(5),
		Effect::modifiedHealth(10),
		Effect::modifiedHealth(20),
		Effect::modifiedHealth(40),
	};
	static_assert(heals.size() == max_types);

private:
	Type mType{};

public:
	Food() = default;
	Food(Type type);

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return heals[mType]; }
	int getHealingAmount() const { return heals[mType].getModifiedHealth(); }

	void description() const override { std::cout << descriptions[mType] << " Heals you by " << getHealingAmount() << "\n"; }
};
