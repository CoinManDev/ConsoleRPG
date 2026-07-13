#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Keybinds.h"
#include "Input.h"
#include "Inventory.h"

void Inventory::displayCategoryNames(const std::vector<Category>& categories)
{
	for (const auto& category : categories)
		std::cout << std::setw(categoryWidth) << category.name;
	std::cout << "\n\n";
}

void Inventory::displayItemAt(const std::vector<Category>& categories, const Point& indicator, std::size_t x, std::size_t y, const Player& player)
{
	if (categories[x].items.size() <= y)
	{
		std::cout << std::setw(nameWidth + indicatorWidth) << " ";
		return;
	}

	const Item* item{ categories[x].items[y] };
	int width{ nameWidth };
	if (item == player.getEquippedWeapon())
	{
		std::cout << "* ";
		width -= 2;
	}

	std::cout << std::setw(width) << item->getName() << std::setw(indicatorWidth) << (indicator.x == x && indicator.y == y ? " <" : " .");
}

void Inventory::displayItems(const std::vector<Category>& categories, const Point& indicator, const Player& player)
{
	std::size_t height{};
	for (const auto& category : categories)
		height = std::max(height, category.items.size());

	for (std::size_t y{}; y < height; ++y)
	{
		for (std::size_t x{}; x < categories.size(); ++x)
		{
			displayItemAt(categories, indicator, x, y, player);
		}

		std::cout << '\n';
	}

	std::cout << '\n';
}

void Inventory::displayInventory(const std::vector<Category>& categories, const Point& indicator, const Player& player)
{
	displayCategoryNames(categories);
	displayItems(categories, indicator, player);
}

void Inventory::displayItemInfo(const Item* item, const Player& player)
{
	if (item)
		item->description();

	if (item == player.getEquippedWeapon())
		std::cout << "\nEquipped\n";
	else if (dynamic_cast<const Weapon*>(item))
		std::cout << "\nPress " << Keybinds::select << " to equip\n";
}

void Inventory::clampIndicator(Point& indicator, const std::vector<Category>& categories)
{
	// categories.size() - 1 might equal -1 if the size is 0 but if it is 0 then we won't really need to show the indicator anyway
	indicator.x = std::clamp(indicator.x, 0, static_cast<int>(categories.size()) - 1);
	indicator.y = std::clamp(indicator.y, 0, static_cast<int>(categories[static_cast<std::size_t>(indicator.x)].items.size()) - 1);
}

void Inventory::addCategories(std::vector<Category>& categories, const Player& player)
{
	addCategory(categories, "Weapons", player.getWeapons());
	addCategory(categories, "Potions", player.getPotions());
	addCategory(categories, "Food", player.getFood());
}

void Inventory::display(const std::vector<Category>& categories, const Point& indicator, const Player& player)
{
	displayInventory(categories, indicator, player);
	displayItemInfo(getIndicatedItem(categories, indicator), player);
}

void Inventory::moveIndicator(Point& indicator, const std::vector<Category>& categories, char direction)
{
	if (direction == Keybinds::up) --indicator.y;
	else if (direction == Keybinds::down) ++indicator.y;
	else if (direction == Keybinds::left) --indicator.x;
	else if (direction == Keybinds::right) ++indicator.x;
	else return;

	clampIndicator(indicator, categories);
}

void Inventory::inventory(Player& player)
{
	Point indicator{};
	std::vector<Category> categories;
	addCategories(categories, player);

	while (true)
	{
		std::cout << Aesthetics::clear << '\n';
		if (categories.empty())
			std::cout << "Your inventory is empty\n";
		else
			display(categories, indicator, player);

		std::cout << "\n( " << Keybinds::movementKeybinds << " to move cursor, " << Keybinds::quit << " to quit inventory )\n";
		char c{ getInput() };

		if (c == Keybinds::quit)
			break;

		if (categories.empty())
			continue;

		if (c == Keybinds::select && dynamic_cast<const Weapon*>(getIndicatedItem(categories, indicator)))
			player.equipWeaponAtIndex(indicator.y);
		else
			moveIndicator(indicator, categories, c);
	}

	std::cout << '\n';
}
