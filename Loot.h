#pragma once

#include "Weapon.h"
#include "Potion.h"
#include "Food.h"

namespace Loot
{
	inline const Weapon dagger{ Weapon::dagger };
	inline const Weapon sword{ Weapon::sword };
	inline const Weapon musket{ Weapon::musket };
	inline const Potion healing{ Potion::healing };
	inline const Potion pain{ Potion::pain };
	inline const Potion strength{ Potion::strength };
	inline const Potion weakness{ Potion::weakness };
	inline const Food bread{ Food::bread };
	inline const Food bread_two{ Food::bread_two };
	inline const Food beef{ Food::beef };
	inline const Food sandvich{ Food::sandvich };

	struct Loot
	{
		const Item* item{};
		int weight;
		bool eligible{ true };
	};

	class LootTable
	{
	private:
		inline static std::vector<std::vector<Loot>> lootTable{
			{ // Tier 1
				Loot{ &dagger, 3 },
				Loot{ &healing, 1 },
				Loot{ &pain, 1 },
				Loot{ &bread, 1 },
			},
			{ // Tier 2
				Loot{ &sword, 3 },
				Loot{ &healing, 1 },
				Loot{ &bread_two, 2 },
			},
			{ // Tier 3
				Loot{ &musket, 3 },
				Loot{ &strength, 1 },
				Loot{ &weakness, 2 },
				Loot{ &beef, 2 },
			},
			{ // Tier 4
				Loot{ &musket, 2 },
				Loot{ &strength, 2 },
				Loot{ &weakness, 2 },
				Loot{ &sandvich, 2 },
			},
		};

	public:
		LootTable() = delete;

		static const std::vector<std::vector<Loot>>& getLootTable() { return lootTable; }
		static void setLootEligibility(std::size_t tierIndex, std::size_t index, bool eligibility) { lootTable[tierIndex][index].eligible = eligibility; }
	};

	bool isTierEligible(std::size_t index);
	void resetTierEligibility(std::size_t index);
	int totalWeightOfTier(std::size_t index);
	const Item* getRandomLoot(int tier);
}
