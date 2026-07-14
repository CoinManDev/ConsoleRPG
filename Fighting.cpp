#include <cassert>
#include "Random.h"
#include "Keybinds.h"
#include "Input.h"
#include "Inventory.h"
#include "Loot.h"
#include "Fighting.h"

void Fighting::attack(FightState& state)
{
	state.enemy.damage(state.player.getDamage());

	if (state.player.getEquippedWeapon())
	{
		state.player.getEquippedWeapon()->useOn(state.enemy);
		int totalDamage{ state.player.getDamage() + state.player.getEquippedWeapon()->getDamage() };
		state.announce = [totalDamage, &state]() { std::cout << "You dealt " << totalDamage << " damage to the " << state.enemy.getName() << '\n'; };
	}
	else
		state.announce = [&]() { std::cout << "You dealt " << state.player.getDamage() << " damage to the " << state.enemy.getName() << '\n'; };
}

void Fighting::displayItemUsage(FightState& state, const Category& category, const Point& indicator)
{
	std::cout << Aesthetics::clear << "Choose an item to use:\n\n";
	Inventory::display({ category }, indicator, state.player);
	std::cout << "\n( " << Keybinds::up << '/' << Keybinds::down << " to move between items, " << Keybinds::select << " to select, " << Keybinds::quit << " to cancel )\n";
}

int Fighting::processInputForItemUsage(FightState& state, const Category& category, Point& indicator, const std::function<void(FightState&, const Item*)>& onItemSelect)
{
	char input{ getInput() };

	if (input == Keybinds::select)
	{
		const Item* item{ Inventory::getIndicatedItem({ category }, indicator) };
		if (item)
		{
			onItemSelect(state, item);
			state.player.removeItem(*item);
			return exit;
		}

		return stay;
	}
	else if (input == Keybinds::quit)
	{
		state.playersTurn = true;
		return exit;
	}
	else
	{
		Inventory::moveIndicator(indicator, { category }, input);
		return stay;
	}
}

void Fighting::useItem(FightState& state, const Category& category, const std::function<void(FightState&)>& onItemAbsence, const std::function<void(FightState&, const Item*)>& onItemSelect)
{
	if (category.items.empty())
	{
		onItemAbsence(state);
		return;
	}

	Point indicator{};

	do {
		displayItemUsage(state, category, indicator);
	} while (processInputForItemUsage(state, category, indicator, onItemSelect) == 0);
}

void Fighting::onPotionAbsence(FightState& state)
{
	state.announce = []() { std::cout << "You have no potions\n"; };
	state.playersTurn = true;
}

void Fighting::selectPotion(FightState& state, const Item* item)
{
	const Potion* potion{ dynamic_cast<const Potion*>(item) };
	if (potion)
	{
		potion->isAppliedOnUser() ? potion->useOn(state.player) : potion->useOn(state.enemy);
		state.announce = [p{ *potion }]() { std::cout << "You used a " << p.getName() << " on " << (p.isAppliedOnUser() ? "yourself " : "the enemy ") << '\n'; };
	}
}

void Fighting::usePotion(FightState& state)
{
	useItem(state, Inventory::getPotionCategory(state.player), onPotionAbsence, selectPotion);
}

void Fighting::onFoodAbsence(FightState& state)
{
	state.announce = []() { std::cout << "You have no food\n"; };
	state.playersTurn = true;
}

void Fighting::selectFood(FightState& state, const Item* item)
{
	const Food* food{ dynamic_cast<const Food*>(item) };
	if (food)
	{
		food->useOn(state.player);
		state.announce = [f{ *food }]() { std::cout << "You ate a " << f.getName() << '\n'; };
	}
}

void Fighting::eatFood(FightState& state)
{
	useItem(state, Inventory::getFoodCategory(state.player), onFoodAbsence, selectFood);
}

void Fighting::tryToFlee(FightState& state)
{
	if (Random::get(1, fleeChance) == 1)
	{
		state.finished = true;
		state.announce = []() { std::cout << "You successfully fled like a coward\n"; };
	}
	else
		state.announce = []() { std::cout << "You couldn't flee\n"; };
}

void Fighting::displayFight(const Player& player, const Enemy& enemy)
{
	int enemyHealth{ enemy.getHealth() < 0 ? 0 : enemy.getHealth() };
	int playerHealth{ player.getHealth() < 0 ? 0 : player.getHealth() };

	std::cout << "\t" << std::setw(displayWidth) << enemy.getSymbol() << player.getSymbol() << "\n\n";
	std::cout << "\t" << "hp: " << std::setw(displayWidth - 4) << enemyHealth << "hp: " << playerHealth << "\n\n"; // 4 is the length of "hp: "
}

void Fighting::displayState(const FightState& state)
{
	std::cout << Aesthetics::clear << '\n';
	displayFight(state.player, state.enemy);

	state.announce();
}

void Fighting::displayOptions(int indicator)
{
	for (std::size_t i{}; i < options.size(); ++i)
		std::cout << (static_cast<std::size_t>(indicator) == i ? "> " : ". ") << std::setw(15) << options[i].name;
}

void Fighting::doAction(FightState& state, int indicator)
{
	state.announce = []() { std::cout << "...\n"; }; // in case action doesn't set an announce ( me ) reset announce to avoid showing the previous one after the screen is cleared in the next loop iteration ( AI )
	state.playersTurn = false; // do it before in case the action modifies playersTurn itself
	options[static_cast<std::size_t>(indicator)].action(state);
}

void Fighting::moveIndicator(int& indicator, char direction)
{
	if (direction == Keybinds::left) --indicator;
	else if (direction == Keybinds::right) ++indicator;

	indicator = std::clamp(indicator, 0, static_cast<int>(options.size()) - 1);
}

void Fighting::enemyAttack(FightState& state)
{
	state.player.setHealth(state.player.getHealth() - state.enemy.getDamage());
	state.announce = [&]() { std::cout << "They dealt " << state.enemy.getDamage() << " damage to you\n"; };
	state.playersTurn = true;
}

void Fighting::playersTurn(FightState& state, int& indicator)
{
	displayOptions(indicator);
	std::cout << "\n\n( " << Keybinds::left << '/' << Keybinds::right << " to move between options, " << Keybinds::select << " to select )\n";

	char c{ getInput() };

	if (c == Keybinds::select)
		doAction(state, indicator);
	else
		moveIndicator(indicator, c);
}

void Fighting::enemysTurn(FightState& state)
{
	waitForInput();
	enemyAttack(state);
}

void Fighting::equipWeaponLoot(const FightState& state, const Weapon* weapon)
{
	std::cout << "Do you want to equip it now? ( " << Keybinds::agree << '/' << Keybinds::disagree << " )\n";
	while (true)
	{
		char input{ getInput() };
		if (input == Keybinds::agree)
		{
			state.player.equipWeaponAtIndex(static_cast<int>(state.player.getWeapons().size()) - 1); // equip the last weapon in the inventory which is the one we just added ( AI suggested this )
			std::cout << "You equipped the " << weapon->getName() << '\n';
			break;
		}
		else if (input == Keybinds::disagree)
		{
			std::cout << "You can equip it whenever you want from your inventory\n";
			break;
		}
	}
}

void Fighting::takeLoot(const FightState& state, const Item* loot)
{
	state.player.addItem(*loot); // ( AI, just keeping it in for fun ) player is guaranteed to outlive the loot since the loot is only used in this function and the player is used in the fight which calls this function and the fight can't end without the player dying or the enemy dying and if the player dies then we won't get to this function and if the enemy dies then we will get to this function but the player will still be alive
	// ( AI again ) also move semantics would be more appropriate here but it would require some changes to the inventory system and I don't want to do that rn
	// can call non const function addItem on const state's member player because player is a reference
	std::cout << "The " << loot->getName() << " has been added to your inventory" << '\n';

	if (const Weapon * weapon{ dynamic_cast<const Weapon*>(loot) })
		equipWeaponLoot(state, weapon);
}

void Fighting::leaveLoot(const Item* loot)
{
	std::cout << "You left the " << loot->getName() << " with the corpse. Maybe out of respect?\n";
}

void Fighting::lootEnemy(const FightState& state)
{
	for (int i{}; i < state.enemy.getTier(); ++i)
	{
		waitForInput();

		const Item* loot = LootTable::getRandomLoot(state.enemy.getTier());
		assert(loot && "Loot is null");
		std::cout << Aesthetics::clear << "The enemy had a " << loot->getName() << ", do you want to take it? ( " << Keybinds::agree << '/' << Keybinds::disagree << " )\n";

		while (true)
		{
			char c{ getInput() };

			if (c == Keybinds::agree)
			{
				takeLoot(state, loot);
				break;
			}
			else if (c == Keybinds::disagree)
			{
				leaveLoot(loot);
				break;
			}
		}
	}
}

void Fighting::conclude(const FightState& state)
{
	if (state.player.isDead())
		std::cout << "You died, quite embarrassingly...\n";
	else if (state.enemy.isDead())
	{
		std::cout << "You killed them, they are now just a soulless husk\n";
		lootEnemy(state);
		state.player.addXp(state.enemy.getXp());
		waitForInput();
	}
}

void Fighting::fight(Player& player, Enemy& enemy)
{
	FightState state{ player, enemy, [&]() { std::cout << "A fight has started between you and the " << enemy.getName() << '\n'; }, false, true };
	int indicator{};

	while (!state.finished && !player.isDead() && !enemy.isDead())
	{
		displayState(state);

		if (state.playersTurn)
			playersTurn(state, indicator);
		else
			enemysTurn(state);
	}

	displayState(state);
	conclude(state);
}
