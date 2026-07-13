#include "Player.h"

Player::Player(Map* map, const Point& position)
	: Creature{ map, position, 'P', Aesthetics::yellow, 10, 0 }
{

}

void Player::addItem(const Item& item)
{
	if (const Weapon * weapon{ dynamic_cast<const Weapon*>(&item) })
		mInventory.weapons.push_back(*weapon);
	else if (const Potion * potion{ dynamic_cast<const Potion*>(&item) })
		mInventory.potions.push_back(*potion);
	else if (const Food * food{ dynamic_cast<const Food*>(&item) })
		mInventory.food.push_back(*food);
}

void Player::removeItem(const Item& item)
{
	if (const Weapon * weapon{ dynamic_cast<const Weapon*>(&item) })
		std::erase_if(mInventory.weapons, [&](const auto& weapon) { return &weapon == &item; });
	else if (const Potion * potion{ dynamic_cast<const Potion*>(&item) })
		std::erase_if(mInventory.potions, [&](const auto& potion) { return &potion == &item; });
	else if (const Food * food{ dynamic_cast<const Food*>(&item) })
		std::erase_if(mInventory.food, [&](const auto& food) { return &food == &item; });
}

void Player::addXp(int amount)
{
	mXp += amount;
	while (mXp >= mRequiredXp)
	{
		mXp -= mRequiredXp;
		mRequiredXp *= 2;
		++mLevel;
		++mDamage;

		std::cout << "You leveled up! You are now level " << mLevel << ". Your damage has been increased by 1\n";
	}
}
