#pragma once

class CreatureBase
{
protected:
	int mHealth{};
	int mDamage{};

public:
	CreatureBase() = default;
	CreatureBase(int health, int damage);
	virtual ~CreatureBase() = default;

	int getHealth() const { return mHealth; }
	void setHealth(int health);

	int getDamage() const { return mDamage; }
	void setDamage(int damage);

	void damage(int amount) { setHealth(mHealth - amount); }
	bool isDead() const { return mHealth <= 0; }
};
