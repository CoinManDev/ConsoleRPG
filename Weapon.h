#pragma once

#include <array>
#include <iostream>
#include "Item.h"

class Weapon : public Item
{
public:
	enum Type
	{
		stick,
		dagger,
		sword,
		musket,

		max_types,
	};

	inline static constexpr std::array names{ "stick", "dagger", "sword", "musket" }; // have to use CTAD here for the static assert to work properly
	static_assert(names.size() == max_types);

	inline static constexpr std::array descriptions{
		"A wooden stick, held by many legendary warriors in the past.",
		"A boring dagger.",
		"A long sword made of iron and love.",
		"A musket which can hurt things or something.",
	};
	static_assert(descriptions.size() == max_types);

	inline static const std::array damages{
		Effect::modifiedHealth(-1),
		Effect::modifiedHealth(-2),
		Effect::modifiedHealth(-4),
		Effect::modifiedHealth(-8),
	}; // have to use CTAD here for the static assert to work properly
	static_assert(damages.size() == max_types);

private:
	Type mType{};

public:
	Weapon() = default;
	Weapon(Type type);

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return damages[mType]; }
	int getDamage() const { return getEffect().getModifiedHealth() * -1; }

	void description() const override { std::cout << descriptions[mType] << " Deals " << getDamage() << " damage\n"; }
};
