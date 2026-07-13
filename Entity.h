#pragma once

#include "Aesthetics.h"
#include "Map.h"

class Entity
{
private:
	Map* mMap;
	Point mPosition{};
	char mSymbol{};
	Aesthetics::Color mColor{};
	bool active{ true };

	void removeFromMap();

public:
	Entity(Map* map, const Point& position, char symbol, Aesthetics::Color color = Aesthetics::white);
	Entity(const Entity& other);
	virtual ~Entity();

	Map* getMap() const { return mMap; }
	const Point& getPosition() const { return mPosition; }
	char getSymbol() const { return mSymbol; }
	Aesthetics::Color getColor() const { return mColor; }

	void setMap(Map* map);

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactivate() { active = false; }

	void moveTo(const Point& to);
	void move(const Point& to);

	friend std::ostream& operator<<(std::ostream& out, const Entity& entity);
};
