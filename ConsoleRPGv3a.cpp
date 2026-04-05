/*#include <array>
#include <cassert>
#include <iostream>
#include <cmath>
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
	const Point left{ -1, 0 };
	const Point right{ 1, 0 };
	const Point up{ 0, -1 };
	const Point down{ 0, 1 };
}

Point distance(const Point& p1, const Point& p2)
{
	return p1 - p2;
}

Point distanceAbs(const Point& p1, const Point& p2)
{
	return Point{ std::abs(p1.x - p2.x), std::abs(p1.y - p2.y) };
}

int sign(int n)
{
	return (n / std::abs(n));
}

template <int width, int height>
class Map;

template <int width, int height>
class Entity
{
private:
	Map<width, height>& mMap;
	Point mPosition{};
	char mSymbol{};

public:
	Entity(Map<width, height>& map, const Point& position, char symbol)
		: mMap{ map }, mPosition{ position }, mSymbol{ symbol }
	{
		assert(!mMap.getEntity(mPosition) && "There's already an entity at that position");

		mMap.setEntity(mPosition, this);
	}

	virtual ~Entity()
	{
		mMap.setEntity(mPosition, nullptr);
	}

	//Entity(const Entity& other) = delete;
	//Entity& operator=(const Entity& other) = delete;

	const Point& getPosition() const { return mPosition; }
	char getSymbol() const { return mSymbol; }

	void moveTo(const Point& to)
	{
		if (!mMap.isPositionInBounds(to))
			return;

		if (!mMap.getEntity(to))
		{
			mMap.setEntity(to, this);
			mMap.setEntity(mPosition, nullptr);
			mPosition = to;
		}
	}

	void move(const Point& to) { moveTo(mPosition + to); }

	friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
	{
		return out << entity.mSymbol;
	}
};

//template <int width, int height>
//Entity( Map<width, height>&map, const Point& position, char symbol ) -> Entity<width, height>;

template <int width, int height>
class Map
{
private:
	std::array<std::array<Entity<width, height>*, width>, height> mEntityMap{};

	Entity<width, height>*& getEntity(const Point& position)
	{
		return mEntityMap[static_cast<std::size_t>(position.y)][static_cast<std::size_t>(position.x)];
	}

	void setEntity(const Point& position, Entity<width, height>* entity)
	{
		getEntity(position) = entity;
	}

	friend Entity<width, height>;

public:
	Map() = default;

	Map(const Map& other) = delete;
	Map& operator=(const Map& other) = delete;

	bool isPositionInBounds(const Point& position)
	{
		return (position.x >= 0 && position.x < width) &&
			   (position.y >= 0 && position.y < height);
	}

	friend std::ostream& operator<<(std::ostream& out, const Map& map)
	{
		for (const auto& row : map.mEntityMap)
		{
			for (const auto* entity : row)
			{
				if (entity)
					out << *entity << ' ';
				else
					out << ". ";
			}

			out << '\n';
		}

		return out;
	}
};

template <int width, int height>
class Player : public Entity<width, height>
{
public:
	Player(Map<width, height>& map, const Point& position)
		: Entity<width, height>{ map, position, 'P' }
	{

	}
};

template <int width, int height>
class Wall : public Entity<width, height>
{
public:
	Wall(Map<width, height>& map, const Point& position)
		: Entity<width, height>{ map, position, '#' }
	{

	}
};

template <int width, int height>
class WallBlock
{
private:
	std::vector<Wall<width, height>> block{};

public:
	WallBlock(Map<width, height>& map, const Point& start, const Point& end)
	{
		Point distAbs{ distanceAbs(start, end) };
		block.reserve(static_cast<std::size_t>((distAbs.x + 1) * (distAbs.y + 1)));

		Point dist{ distance(start, end) };
		//for (int y{}; (dist.y < 0 ? y >= dist.y : y <= dist.y); (dist.y < 0 ? --y : ++y))
		//	for (int x{}; (dist.x < 0 ? x >= dist.x : x <= dist.x); (dist.x < 0 ? --x : ++x))
		//		block.emplace_back(map, start + Point{ -x, -y });
		for (int y{}; y <= distAbs.y; ++y)
			for (int x{}; x <= distAbs.x; ++x)
				block.emplace_back(map, start - Point{ x * sign(dist.x), y * sign(dist.y) });
	}
};

int main()
{
	Map<10, 10> map{};

	std::cout << map << '\n';

	Entity e{ map, Point{ 5, 5 }, 'E' };

	std::cout << map << '\n';

	Player p{ map, Point{ 1, 1 } };

	std::cout << map << '\n';

	p.moveTo(p.getPosition() + Directions::down);

	std::cout << map << '\n';

	//Entity e2{ e };
	//e2.moveTo(Point{ 3, 3 });

	//std::cout << map << '\n';

	//WallBlock block{ map, Point{ 1, 7 }, Point{ 2, 8 } };
	WallBlock block{ map, Point{ 2, 8 }, Point{ 1, 7 } };
	WallBlock block2{ map, Point{ 6, 6 }, Point{ 3, 8 } };
	WallBlock block3{ map, Point{ 8, 0 }, Point{ 9, 1 } };

	while (true)
	{
		std::cout << "Enter a direction: ";
		char c{};
		std::cin >> c;

		for (int i{}; i < 20; ++i)
			std::cout << '\n';

		Point dir{};
		switch (c)
		{
		case 'w': dir = Directions::up; break;
		case 'a': dir = Directions::left; break;
		case 's': dir = Directions::down; break;
		case 'd': dir = Directions::right; break;
		}

		p.move(dir);
		std::cout << map << '\n';
	}

	return 0;
}*/
