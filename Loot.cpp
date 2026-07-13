#include <cassert>
#include "Random.h"
#include "Loot.h"

bool Loot::isTierEligible(std::size_t index)
{
	for (const auto& loot : LootTable::getLootTable()[index])
		if (loot.eligible)
			return true;
	return false;
}

void Loot::resetTierEligibility(std::size_t index)
{
	for (std::size_t i{}; i < LootTable::getLootTable()[index].size(); ++i)
		LootTable::setLootEligibility(index, i, true);
}

int Loot::totalWeightOfTier(std::size_t index)
{
	int totalWeight{};
	for (const auto& loot : LootTable::getLootTable()[index])
		totalWeight += (loot.weight * static_cast<int>(loot.eligible));
	return totalWeight;
}

const Item* Loot::getRandomLoot(int tier)
{
	assert((tier > 0 && tier <= ssize(LootTable::getLootTable())) && "Invalid tier for loot table");
	std::size_t stier{ static_cast<std::size_t>(tier - 1) };
	if (isTierEligible(stier) == false)
		resetTierEligibility(stier);

	int totalTierWeight{ totalWeightOfTier(stier) };
	int chance{ Random::get(1, totalTierWeight) };
	int accumulativeWeight{};
	for (std::size_t i{}; i < LootTable::getLootTable()[stier].size(); ++i)
	{
		const Loot& loot{ LootTable::getLootTable()[stier][i] };

		if (!loot.eligible)
			continue;

		accumulativeWeight += loot.weight;
		if (chance <= accumulativeWeight)
		{
			LootTable::setLootEligibility(stier, i, false);
			return loot.item;
		}
	}

	return nullptr;
}
