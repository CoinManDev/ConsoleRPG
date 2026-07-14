#include <cassert>
#include "Random.h"
#include "Loot.h"

bool LootTable::isTierEligible(std::size_t index)
{
	for (const auto& loot : table[index])
		if (loot.eligible)
			return true;
	return false;
}

void LootTable::resetTierEligibility(std::size_t index)
{
	for (auto& loot : table[index])
		loot.eligible = true;
}

int LootTable::totalWeightOfTier(std::size_t index)
{
	int totalWeight{};
	for (const auto& loot : table[index])
		totalWeight += (loot.weight * static_cast<int>(loot.eligible));
	return totalWeight;
}

const Item* LootTable::getRandomLoot(int tier)
{
	assert((tier > 0 && tier <= ssize(table)) && "Invalid tier for loot table");
	std::size_t stier{ static_cast<std::size_t>(tier - 1) };
	if (isTierEligible(stier) == false)
		resetTierEligibility(stier);

	int totalTierWeight{ totalWeightOfTier(stier) };
	int chance{ Random::get(1, totalTierWeight) };
	int accumulativeWeight{};
	for (auto& loot : table[stier])
	{
		if (!loot.eligible)
			continue;

		accumulativeWeight += loot.weight;
		if (chance <= accumulativeWeight)
		{
			loot.eligible = false;
			return loot.item;
		}
	}

	return nullptr;
}
