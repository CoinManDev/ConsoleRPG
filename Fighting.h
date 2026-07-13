#pragma once

#include <functional>
#include "Player.h"
#include "Enemy.h"
#include "Category.h"

namespace Fighting
{
	constexpr int fleeChance{ 3 };
	constexpr int displayWidth{ 10 };
	constexpr int exit{ 1 };
	constexpr int stay{ 0 };

	struct FightState
	{
		Player& player;
		Enemy& enemy;
		std::function<void()> announce;
		bool finished{ false };
		bool playersTurn{ true };
	};

	struct Option
	{
		std::string name{};
		std::function<void(FightState&)> action{};
	};

	void attack(FightState& state);
	void displayItemUsage(FightState& state, const Category& category, const Point& indicator);
	int processInputForItemUsage(FightState& state, const Category& category, Point& indicator, const std::function<void(FightState&, const Item*)>& onItemSelect);
	void useItem(FightState& state, const Category& category, const std::function<void(FightState&)>& onItemAbsence, const std::function<void(FightState&, const Item*)>& onItemSelect);
	void onPotionAbsence(FightState& state);
	void selectPotion(FightState& state, const Item* item);
	void usePotion(FightState& state);
	void onFoodAbsence(FightState& state);
	void selectFood(FightState& state, const Item* item);
	void eatFood(FightState& state);
	void tryToFlee(FightState& state);

	const std::array options{
		Option{ "attack", attack },
		Option{ "use potion", usePotion },
		Option{ "eat food", eatFood },
		Option{ "try to flee", tryToFlee },
	};

	void displayFight(const Player& player, const Enemy& enemy);
	void displayState(const FightState& state);
	void displayOptions(int indicator);
	void doAction(FightState& state, int indicator);
	void moveIndicator(int& indicator, char direction);
	void enemyAttack(FightState& state);
	void playersTurn(FightState& state, int& indicator);
	void enemysTurn(FightState& state);
	void equipWeaponLoot(const FightState& state, const Weapon* weapon);
	void takeLoot(const FightState& state, const Item* loot);
	void leaveLoot(const Item* loot);
	void lootEnemy(const FightState& state);
	void conclude(const FightState& state);
	void fight(Player& player, Enemy& enemy);
}
