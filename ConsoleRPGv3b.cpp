/*#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <conio.h>

namespace Aesthetics
{
	enum Color
	{
		reset_color,

		black,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,

		black_bg,
		red_bg,
		green_bg,
		yellow_bg,
		blue_bg,
		magenta_bg,
		cyan_bg,
		white_bg,

		max_colors,
	};

	constexpr std::array colors{
		"\033[0m",

		"\033[30m",
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",

		"\033[40m",
		"\033[41m",
		"\033[42m",
		"\033[43m",
		"\033[44m",
		"\033[45m",
		"\033[46m",
		"\033[47m",
	};
	static_assert(colors.size() == max_colors);

	constexpr Color bgColor{ green_bg };
}

std::ostream& operator<<(std::ostream& out, Aesthetics::Color color)
{
	return out << Aesthetics::colors[color];
}

struct Point
{
	int x{};
	int y{};
};

std::ostream& operator<<(std::ostream& out, const Point& point)
{
	return out << "x: " << point.x << ", y: " << point.y;
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}

bool operator==(const Point& p1, const Point& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

bool operator!=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
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
	Aesthetics::Color mColor{};

public:
	Entity(Map<width, height>& map, const Point& position, char symbol, Aesthetics::Color color = Aesthetics::white)
		: mMap{ map }, mPosition{ position }, mSymbol{ symbol }, mColor{ color }
	{
		mMap.addEntity(mPosition, this);
	}

	virtual ~Entity()
	{
		mMap.removeEntity(mPosition, this);
	}

	Entity(const Entity& other)
		: Entity{ other.mMap, other.mPosition, other.mSymbol, other.mColor }
	{

	}

	//Entity& operator=(const Entity& other)
	//{
	//	if (this == &other)
	//		return *this;
	//	
	//	moveTo(other.mPosition);
	//	mSymbol = other.mSymbol;
	//
	//	return *this;
	//}
	Entity& operator=(const Entity& other) = delete;

	const Point& getPosition() const { return mPosition; }
	char getSymbol() const { return mSymbol; }
	Aesthetics::Color getColor() const { return mColor; }

	virtual std::unique_ptr<Entity> clone() const
	{
		return std::make_unique<Entity>(*this);
	}

	void moveTo(const Point& to)
	{
		if (!mMap.isPositionInBounds(to))
			return;

		mMap.removeEntity(mPosition, this); // remove first to account for the scenario when mPosition and to is same ( if we added first removeEntity would remove the instance just added as well )
		mMap.addEntity(to, this);
		mPosition = to;
	}

	void move(const Point& to) { moveTo(mPosition + to); }

	friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
	{
		return out << entity.mColor << Aesthetics::bgColor << entity.mSymbol;
	}
};

//template <int width, int height>
//Entity( Map<width, height>&map, const Point& position, char symbol ) -> Entity<width, height>;

template <int width, int height>
class Map
{
private:
	std::array<std::array<std::vector<Entity<width, height>*>, width>, height> mEntityMap{};

	std::vector<Entity<width, height>*>& getEntitySlot(const Point& position)
	{
		return mEntityMap[static_cast<std::size_t>(position.y)][static_cast<std::size_t>(position.x)];
	}

	void addEntity(const Point& position, Entity<width, height>* entity)
	{
		getEntitySlot(position).push_back(entity);
	}

	void removeEntity(const Point& position, Entity<width, height>* entity)
	{
		std::erase(getEntitySlot(position), entity); // removes ALL instances of entity
	}

	friend Entity<width, height>;

public:
	Map() = default;

	Map(const Map& other) = delete;
	Map& operator=(const Map& other) = delete;

	//const Entity<width, height>* getEntity(const Point& position)
	Entity<width, height>* getEntity(const Point& position)
	{
		if (!isPositionInBounds(position))
			return nullptr;

		const auto& slot{ getEntitySlot(position) };
		if (slot.empty())
			return nullptr;
		else
			return slot[0];
	}

	bool isPositionInBounds(const Point& position)
	{
		return (position.x >= 0 && position.x < width) &&
			   (position.y >= 0 && position.y < height);
	}

	friend std::ostream& operator<<(std::ostream& out, const Map& map)
	{
		for (const auto& row : map.mEntityMap)
		{
			for (const auto& slot : row)
			{
				if (!slot.empty())
					out << *slot[0] << ' ' << Aesthetics::reset_color;
				else
					out << Aesthetics::bgColor << "  " << Aesthetics::reset_color;
			}

			out << '\n';
		}

		return out;
	}
};

class CreatureBase
{
private:
	int mHealth{};
	int mDamage{};

public:
	CreatureBase() = default;

	CreatureBase(int health, int damage)
		: mHealth{ health }, mDamage{ damage }
	{

	}

	virtual ~CreatureBase() = default;

	int getHealth() const { return mHealth; }
	void setHealth(int health) { mHealth = health; }

	int getDamage() const { return mDamage; }
	void setDamage(int damage) { mDamage = damage; }
};

class Effect
{
private:
	CreatureBase mModification{};

public:
	Effect(const CreatureBase& modification)
		: mModification{ modification }
	{

	}

	const CreatureBase& getModification() const { return mModification; }

	void applyOn(CreatureBase& victim) const
	{
		victim.setHealth(victim.getHealth() + mModification.getHealth());
		victim.setDamage(victim.getDamage() + mModification.getDamage());
	}

	static Effect modifiedHealth(int amount)
	{
		return Effect{ CreatureBase{ amount, 0 } };
	}

	static Effect modifiedDamage(int amount)
	{
		return Effect{ CreatureBase{ 0, amount } };
	}
};

template <int width, int height>
class Creature : public Entity<width, height>, public CreatureBase
{
public:
	Creature(Map<width, height>& map, const Point& position, char symbol, Aesthetics::Color color, int health, int damage)
		: Entity<width, height>{ map, position, symbol, color }, CreatureBase{ health, damage }
	{

	}

	std::unique_ptr<Entity<width, height>> clone() const override
	{
		return std::make_unique<Creature>(*this);
	}
};

class Item
{
public:
	virtual const char* getName() const = 0;
	virtual const Effect& getEffect() const = 0;

	void useOn(CreatureBase& on)
	{
		getEffect().applyOn(on);
	}
};

class Weapon : public Item
{
public:
	enum Type
	{
		stick,
		dagger,
		sword,
		musket,

		max_types,
	};

	inline static constexpr std::array names{ "stick", "dagger", "sword", "musket" }; // have to use CTAD here for the static assert to work properly
	static_assert(names.size() == max_types);

	inline static const std::array damages{
		Effect::modifiedHealth(-1),
		Effect::modifiedHealth(-2),
		Effect::modifiedHealth(-4),
		Effect::modifiedHealth(-8),
	}; // have to use CTAD here for the static assert to work properly
	static_assert(damages.size() == max_types);

private:
	Type mType{};

public:
	Weapon(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	//int getDamage() const { return damages[mType].getModification().getDamage(); }
	const Effect& getEffect() const override { return damages[mType]; }
};

class Potion : public Item
{
public:
	enum Type
	{
		healing,
		pain,
		strength,
		weakness,

		max_types,
	};

	inline static constexpr std::array names{ "potion of healing", "potion of pain and suffering", "potion of strength", "potion of weakness"};
	static_assert(names.size() == max_types);

	inline static const std::array effects{
		//std::function<void(CreatureBase&)>{ [](auto& c) { c.setHealth(c.getHealth() + 5); } },
		//std::function<void(CreatureBase&)>{ [](auto& c) { c.setHealth(c.getHealth() - 5); } },
		//std::function<void(CreatureBase&)>{ [](auto& c) { c.setDamage(c.getDamage() - 2); } },
		Effect::modifiedHealth(5),
		Effect::modifiedHealth(-5),
		Effect::modifiedDamage(2),
		Effect::modifiedDamage(-2),
	};
	static_assert(effects.size() == max_types);

private:
	Type mType{};

public:
	Potion(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	//const std::function<void(CreatureBase&)>& getEffect() const { return effects[mType]; }
	const Effect& getEffect() const override { return effects[mType]; }
};

class Food : public Item
{
public:
	enum Type
	{
		bread,
		bread_two,
		beef,
		sandvich,

		max_types,
	};

	inline static constexpr std::array names{ "bread", "bread two", "beef", "sandvich" };
	static_assert(names.size() == max_types);

	inline static const std::array heals{
		Effect::modifiedHealth(1),
		Effect::modifiedHealth(2),
		Effect::modifiedHealth(4),
		Effect::modifiedHealth(8),
	};
	static_assert(heals.size() == max_types);

private:
	Type mType{};

public:
	Food(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return heals[mType]; }
};

template <int width, int height>
class Player : public Creature<width, height>
{
private:
	Item* mItem{};

public:
	Player(Map<width, height>& map, const Point& position)
		: Creature<width, height>{ map, position, 'P', Aesthetics::yellow, 10, 1 }
	{

	}

	std::unique_ptr<Entity<width, height>> clone() const override
	{
		return std::make_unique<Player>(*this);
	}

	Item* getItem() { return mItem; }
	void setItem(Item& item) { mItem = &item; }
};

template <int width, int height>
class Enemy : public Creature<width, height>
{
public:
	Enemy(Map<width, height>& map, const Point& position)
		: Creature<width, height>{ map, position, 'e', Aesthetics::red, 10, 1 }
	{

	}

	std::unique_ptr<Entity<width, height>> clone() const override
	{
		return std::make_unique<Enemy>(*this);
	}
};

template <int width, int height>
class Wall : public Entity<width, height>
{
public:
	Wall(Map<width, height>& map, const Point& position)
		: Entity<width, height>{ map, position, '#', Aesthetics::magenta }
	{

	}

	std::unique_ptr<Entity<width, height>> clone() const override
	{
		return std::make_unique<Wall>(*this);
	}
};

template <int width, int height>
class Block
{
private:
	std::vector<std::vector<std::unique_ptr<Entity<width, height>>>> mBlock{};
	Point mStart{};

public:
	Block(const Entity<width, height>& sample, const Point& start, const Point& end, const Point& gap = {})
		: mStart{ start }
	{
		Point distAbs{ distanceAbs(start, end) };
		mBlock.resize(static_cast<std::size_t>(distAbs.y + 1));
		for (auto& row : mBlock)
			row.resize(static_cast<std::size_t>(distAbs.x + 1));

		Point dist{ distance(start, end) };
		for (int y{}; y <= distAbs.y; y += (1 + gap.y))
		{
			for (int x{}; x <= distAbs.x; x += (1 + gap.x))
			{
				auto& ptr{ mBlock[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] };
				ptr = sample.clone(); // clone method suggested by ChatGPT
				ptr->moveTo(start - Point{ x * sign(dist.x), y * sign(dist.y) });
			}
		}
	}

	Entity<width, height>& getEntity(const Point& position)
	{
		//const auto ptr{std::find_if(mBlock.begin(), mBlock.end(), [position](const auto& ptr) {
		//	if (ptr)
		//		return ptr->getPosition() == position;
		//	else
		//		return false;
		//	}) };
		//assert(ptr != mBlock.end() && "Couldn't find entity in block");
		//
		//return **ptr;
		Point distAbs{ distanceAbs(position, mStart) };
		auto& ptr{ mBlock[static_cast<std::size_t>(distAbs.y)][static_cast<std::size_t>(distAbs.x)] };
		assert(ptr && "Couldn't find entity in block");
		assert(ptr->getPosition() == position && "Faulty position in getEntity");

		return *ptr;
	}
};

char getInput()
{
	return static_cast<char>(_getch());
}

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
	Block block{ Wall{ map, Point{} }, Point{2, 8}, Point{1, 7}};
	Block block2{ Wall{ map, Point{} }, Point{ 6, 6 }, Point{ 3, 8 } };
	Block block3{ Wall{ map, Point{} }, Point{ 8, 0 }, Point{ 9, 1 } };

	Entity e2{ e };
	e.move(Directions::left);

	std::cout << map << '\n';
	
	//e2 = p;

	//std::cout << map << '\n';

	Block block4{ Entity{ map, Point{}, 'E' }, Point{ 0, 0 }, Point{ 2, 2 }, Point{ 1, 1 } };

	std::cout << map << '\n';

	Map<20, 20> map2{};

	Block block5{ Entity{ map2, Point{}, 'E' }, Point{ 0, 0 }, Point{ 10, 10 }, Point{ 1, 1 } };

	std::cout << map2 << '\n';

	Block block6{ Entity{ map2, Point{}, 'E' }, Point{ 1, 1 }, Point{ 9, 11 }, Point{ 1, 1 } };

	std::cout << map2 << '\n';

	std::cout << block6.getEntity(Point{ 1, 1 }).getPosition() << '\n';
	//std::cout << block6.getEntity(Point{ 2, 2 }).getPosition() << '\n'; SHOULD ASSERT OUT
	//std::cout << block6.getEntity(Point{ -1, -1 }).getPosition() << '\n'; SHOULD ASSERT OUT

	Enemy enemy{ map, Point{ 0, 7 } };

	Weapon weapon{ Weapon::sword };
	p.setItem(weapon);

	while (true)
	{
		std::cout << "Enter a direction: ";
		char c{ getInput() };
		std::cout << "\033[H\033[2J" << '\n';
		//for (int i{}; i < 20; ++i)
		//	std::cout << '\n';

		Point dir{};
		switch (c)
		{
		case 'w': dir = Directions::up; break;
		case 'a': dir = Directions::left; break;
		case 's': dir = Directions::down; break;
		case 'd': dir = Directions::right; break;
		}

		auto* entity{ map.getEntity(p.getPosition() + dir) };
		if (!entity)
			p.move(dir);
		else if (entity->getSymbol() == 'e')
		{
			CreatureBase* creature{ dynamic_cast<Enemy<10, 10>*>(entity) };
			p.getItem()->useOn(*creature);

			std::cout << "Enemy has " << creature->getHealth() << " health left.\n";
		}

		std::cout << map << '\n';
	}

	return 0;
}*/
