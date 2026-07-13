#include "Effect.h"

Effect::Effect(const CreatureBase& modification)
	: mModification{ modification }
{

}

void Effect::applyOn(CreatureBase& victim) const
{
	victim.setHealth(victim.getHealth() + mModification.getHealth());
	victim.setDamage(victim.getDamage() + mModification.getDamage());
}
