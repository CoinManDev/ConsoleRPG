#include "CreatureBase.h"

CreatureBase::CreatureBase(int health, int damage)
	: mHealth{ health }, mDamage{ damage }
{

}

void CreatureBase::setHealth(int health)
{
	mHealth = health;
	if (mHealth < 0)
		mHealth = 0;
}

void CreatureBase::setDamage(int damage)
{
	mDamage = damage;
	if (mDamage < 0)
		mDamage = 0;
}
