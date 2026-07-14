/*
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

struct Point
{
	int x{};
	int y{};
};

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}

namespace Directions
{
	constexpr Point right{ 1, 0 };
	constexpr Point left{ -1, 0 };
	constexpr Point down{ 0, 1 };
	constexpr Point up{ 0, -1 };
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
	return out << "x: " << p.x << " y: " << p.y;
}

class Entity;

class Slot
{
private:
	Entity* mValue{};
	//Point mPosition{};

public:
	Entity* getValue() const { return mValue; }
	void setValue(Entity* value) { mValue = value; }
	//bool isEmpty() const { return mValue == nullptr; }

	//const Point& getPosition() const { return mPosition; }
	//void setPosition(const Point& p) { mPosition = p; }
};

class Entity
{
private:
	std::reference_wrapper<Slot> mSlot;
	char mSymbol{};

public:
	Entity(Slot& slot, char symbol)
		: mSlot{ slot }, mSymbol{ symbol }
	{
		mSlot.get().setValue(this);
	}

	virtual ~Entity()
	{
		mSlot.get().setValue(nullptr);
	}

	//const Point& getPosition() const { return mSlot.getPosition(); }
	char getSymbol() const { return mSymbol; }

	void rebind(Slot& slot) { mSlot = slot; }
};

std::ostream& operator<<(std::ostream& out, const Entity& entity)
{
	return out << entity.getSymbol();
}

template <int width, int height>
class Map
{
private:
	std::array<std::array<Slot, width>, height> mEntityMap{};

public:
	//Map()
	//{
	//	for (int y{}; y < height; ++y)
	//		for (int x{}; x < width; ++x)
	//			getSlot(Point{ x, y }).setPosition(Point{x, y});
	//}

	bool isPointInBounds(const Point& point) const
	{
		return (point.x >= 0 && point.x < width) &&
			   (point.y >= 0 && point.y < height);
	}

	Slot& getSlot(const Point& position)
	{
		//assert(isPointInBounds(position) && "Position isn't in-bounds");
		return mEntityMap[static_cast<std::size_t>(position.y)][static_cast<std::size_t>(position.x)];
	}

	void move(const Point& from, const Point& to)
	{
		Slot& src{ getSlot(from) };
		Slot& dest{ getSlot(to) };
		if (!dest.getValue())
		{
			src.getValue()->rebind(dest);
			std::swap(src, dest);
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Map<width, height>& map)
	{
		for (const auto& row : map.mEntityMap)
		{
			for (const auto& slot : row)
			{
				if (slot.getValue())
					out << *slot.getValue() << ' ';
				else
					out << ". ";
			}

			out << '\n';
		}

		return out;
	}
};

class Player : public Entity
{
public:
	Player(Slot& slot)
		: Entity(slot, 'P')
	{

	}
};

int main()
{
	Map<10, 10> map{};
	Entity e{ map.getSlot(Point{ 5, 5 }), 'E' };

	{
		Entity e2{ map.getSlot(Point{ 2, 2 }), 'E' };
	}

	Entity e3{ map.getSlot(Point{ 2, 2 }), 'E' };

	Player p{ map.getSlot(Point{ 8, 4 }) };

	std::cout << map << '\n';

	map.move(Point{ 8, 4 }, Point{ 8, 4 } + Directions::down);

	std::cout << map << '\n';

	{
		Entity e4{ map.getSlot(Point{ 4, 8 }), 'E' };

		std::cout << map << '\n';

		map.move(Point{ 4, 8 }, Point{ 4, 8 } + Directions::left);

		std::cout << map << '\n';
	}

	//std::cout << map.getSlot(Point{ 4, 8 }).getValue() << ' ' << map.getSlot(Point{4, 8}).getValue()->getSymbol() << '\n';

	std::cout << map << '\n';

	return 0;
}
*/
