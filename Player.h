#pragma once

#include <vector>
#include "Point.h"
#include "Creature.h"
#include "Weapon.h"
#include "Potion.h"
#include "Food.h"

class Player : public Creature
{
public:
	struct Inventory
	{
		std::vector<Weapon> weapons{};
		std::vector<Potion> potions{};
		std::vector<Food> food{};
		std::size_t equippedWeaponIndex{};
	};

private:
	Inventory mInventory{};
	int mXp{};
	int mRequiredXp{ 50 };
	int mLevel{};

public:
	Player(Map* map, const Point& position);

	const std::vector<Weapon>& getWeapons() const { return mInventory.weapons; }
	const std::vector<Potion>& getPotions() const { return mInventory.potions; }
	const std::vector<Food>& getFood() const { return mInventory.food; }

	const Weapon* getEquippedWeapon() const { return getWeapons().empty() ? nullptr : &getWeapons()[mInventory.equippedWeaponIndex]; }
	void equipWeaponAtIndex(int index) { mInventory.equippedWeaponIndex = static_cast<std::size_t>(index); }

	const Inventory& getInventory() const { return mInventory; }
	void setInventory(const Inventory& inventory) { mInventory = inventory; }

	int getLevel() const { return mLevel; }

	void addItem(const Item& item);
	void removeItem(const Item& item);
	void addXp(int amount);
};
