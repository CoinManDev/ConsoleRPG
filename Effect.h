#pragma once

#include "CreatureBase.h"

class Effect
{
private:
	CreatureBase mModification{};

public:
	Effect(const CreatureBase& modification);

	const CreatureBase& getModification() const { return mModification; }
	int getModifiedHealth() const { return mModification.getHealth(); }
	int getModifiedDamage() const { return mModification.getDamage(); }

	void applyOn(CreatureBase& victim) const;

	static Effect modifiedHealth(int amount) { return Effect{ CreatureBase{ amount, 0 } }; }
	static Effect modifiedDamage(int amount) { return Effect{ CreatureBase{ 0, amount } }; }
};
