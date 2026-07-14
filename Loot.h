#pragma once

#include "Weapon.h"
#include "Potion.h"
#include "Food.h"

class LootTable
{
public:
	inline static const Weapon dagger{ Weapon::dagger };
	inline static const Weapon sword{ Weapon::sword };
	inline static const Weapon musket{ Weapon::musket };
	inline static const Potion healing{ Potion::healing };
	inline static const Potion pain{ Potion::pain };
	inline static const Potion strength{ Potion::strength };
	inline static const Potion weakness{ Potion::weakness };
	inline static const Food bread{ Food::bread };
	inline static const Food bread_two{ Food::bread_two };
	inline static const Food beef{ Food::beef };
	inline static const Food sandvich{ Food::sandvich };

	struct Loot
	{
		const Item* item{};
		int weight;
		bool eligible{ true };
	};

private:
	inline static std::vector<std::vector<Loot>> table{
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

	static bool isTierEligible(std::size_t index);
	static void resetTierEligibility(std::size_t index);
	static int totalWeightOfTier(std::size_t index);
	static const Item* getRandomLoot(int tier);
};
