#pragma once

#include <vector>
#include "Player.h"
#include "Category.h"

namespace Inventory
{
	constexpr int nameWidth{ 20 };
	constexpr int indicatorWidth{ 10 };
	constexpr int categoryWidth{ nameWidth + indicatorWidth };

	template <typename T>
	void addCategory(std::vector<Category>& categories, std::string_view name, const std::vector<T>& items)
	{
		if (items.empty())
			return;

		categories.push_back(createCategory(items, name));
	}

	inline Category getWeaponCategory(const Player& player) { return createCategory(player.getWeapons(), "Weapons"); }
	inline Category getPotionCategory(const Player& player) { return createCategory(player.getPotions(), "Potions"); }
	inline Category getFoodCategory(const Player& player) { return createCategory(player.getFood(), "Food"); }
	inline const Item* getIndicatedItem(const std::vector<Category>& categories, const Point& indicator) { return categories[static_cast<std::size_t>(indicator.x)].items[static_cast<std::size_t>(indicator.y)]; }

	void displayCategoryNames(const std::vector<Category>& categories);
	void displayItemAt(const std::vector<Category>& categories, const Point& indicator, std::size_t x, std::size_t y, const Player& player);
	void displayItems(const std::vector<Category>& categories, const Point& indicator, const Player& player);
	void displayInventory(const std::vector<Category>& categories, const Point& indicator, const Player& player);
	void displayItemInfo(const Item* item, const Player& player);
	void clampIndicator(Point& indicator, const std::vector<Category>& categories);
	void addCategories(std::vector<Category>& categories, const Player& player);
	void display(const std::vector<Category>& categories, const Point& indicator, const Player& player);
	void moveIndicator(Point& indicator, const std::vector<Category>& categories, char direction);
	void inventory(Player& player);
};
