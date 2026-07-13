#pragma once

#include <array>
#include <iostream>
#include "Item.h"

class Potion : public Item
{
public:
	enum Type
	{
		healing,
		pain,
		strength,
		weakness,

		max_types,
	};

	inline static constexpr std::array names{ "potion of healing", "potion of pain and suffering", "potion of strength", "potion of weakness" };
	static_assert(names.size() == max_types);

	inline static constexpr std::array descriptions{
		"A red potion and not green for some reason. Increases your health by ",
		"A green potion and not red for some reason. Reduces their health by ",
		"A potion that you can't see the color of because it's in an opaque bottle. Increases your damage by ",
		"It's just beer. Reduces their damage by ",
	};
	static_assert(descriptions.size() == max_types);

	inline static const std::array effects{
		Effect::modifiedHealth(5),
		Effect::modifiedHealth(-5),
		Effect::modifiedDamage(2),
		Effect::modifiedDamage(-2),
	};
	static_assert(effects.size() == max_types);

	inline static constexpr std::array applyOnUser{ true, false, true, false };

private:
	Type mType{};

public:
	Potion() = default;
	Potion(Type type);

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return effects[mType]; }

	void description() const override { std::cout << descriptions[mType] << (getEffect().getModifiedHealth() == 0 ? getEffect().getModifiedDamage() : getEffect().getModifiedHealth()) << "\n"; }

	bool isAppliedOnUser() const { return applyOnUser[mType]; }
};
