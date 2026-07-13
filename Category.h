#pragma once

#include <string>
#include <vector>
#include "Item.h"

struct Category
{
	std::vector<const Item*> items;
	std::string name;
};

template <typename T>
Category createCategory(const std::vector<T>& items, std::string_view name)
{
	Category category{};

	category.name = name;
	category.items.reserve(items.size());
	for (const auto& t : items)
		category.items.push_back(&t);

	return category;
}
