#pragma once

#include "Effect.h"
#include "CreatureBase.h"

class Item
{
public:
	virtual const char* getName() const = 0;
	virtual const Effect& getEffect() const = 0;
	virtual void description() const = 0;

	virtual ~Item() = default;

	void useOn(CreatureBase& on) const { getEffect().applyOn(on); }
};
