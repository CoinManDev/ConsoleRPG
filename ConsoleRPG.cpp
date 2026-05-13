#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
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

	constexpr const char* clear{ "\033[H\033[2J" };
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

class Entity;

class Map
{
private:
	std::vector<std::vector<std::vector<Entity*>>> mEntityMap{};
	const int mWidth{};
	const int mHeight{};

	std::vector<Entity*>& getEntitySlot(const Point& position)
	{
		return mEntityMap[static_cast<std::size_t>(position.y)][static_cast<std::size_t>(position.x)];
	}

	void addEntity(const Point& position, Entity* entity)
	{
		getEntitySlot(position).push_back(entity);
	}

	void removeEntity(const Point& position, Entity* entity)
	{
		std::erase(getEntitySlot(position), entity); // removes ALL instances of entity
	}

	friend Entity;

public:
	Map(int width, int height)
		: mWidth{ width }, mHeight{ height }
	{
		assert((width >= 0 && height >= 0) && "Map dimensions smaller than zero");

		mEntityMap.resize(static_cast<std::size_t>(height));
		for (auto& row : mEntityMap)
			row.resize(static_cast<std::size_t>(width));
	}

	Map(const Map& other) = delete;
	Map& operator=(const Map& other) = delete;

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

	Entity* getEntityAtSlot(const std::vector<Entity*>& slot) const;

	Entity* getEntity(const Point& position)
	{
		if (!isPositionInBounds(position))
			return nullptr;

		return getEntityAtSlot(getEntitySlot(position));
	}

	bool isPositionInBounds(const Point& position) const
	{
		return (position.x >= 0 && position.x < mWidth) &&
			(position.y >= 0 && position.y < mHeight);
	}

	friend std::ostream& operator<<(std::ostream& out, const Map& map);
};

class Entity
{
private:
	Map& mMap;
	Point mPosition{};
	char mSymbol{};
	Aesthetics::Color mColor{};
	bool active{ true };

	void removeFromMap() { mMap.removeEntity(mPosition, this); }

public:
	Entity(Map& map, const Point& position, char symbol, Aesthetics::Color color = Aesthetics::white)
		: mMap{ map }, mPosition{ position }, mSymbol{ symbol }, mColor{ color }
	{
		mMap.addEntity(mPosition, this);
	}

	virtual ~Entity()
	{
		removeFromMap();
	}

	Entity(const Entity& other)
		: Entity{ other.mMap, other.mPosition, other.mSymbol, other.mColor }
	{

	}

	Entity& operator=(const Entity& other) = delete;

	const Point& getPosition() const { return mPosition; }
	char getSymbol() const { return mSymbol; }
	Aesthetics::Color getColor() const { return mColor; }

	virtual std::unique_ptr<Entity> clone() const
	{
		return std::make_unique<Entity>(*this);
	}

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactivate() { active = false; }

	void moveTo(const Point& to)
	{
		if (!mMap.isPositionInBounds(to))
			return;

		removeFromMap(); // remove first to account for the scenario when mPosition and to is same ( if we added first removeEntity would remove the instance just added as well )
		mMap.addEntity(to, this);
		mPosition = to;
	}

	void move(const Point& to) { moveTo(mPosition + to); }

	friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
	{
		return out << entity.mColor << Aesthetics::bgColor << entity.mSymbol;
	}
};

Entity* Map::getEntityAtSlot(const std::vector<Entity*>& slot) const
{
	for (auto* entity : slot)
		if (entity->isActive())
			return entity;

	return nullptr;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
	for (const auto& row : map.mEntityMap)
	{
		for (const auto& slot : row)
		{
			Entity* entity{ map.getEntityAtSlot(slot) };
			if (entity)
				out << *entity << ' ' << Aesthetics::reset_color;
			else
				out << Aesthetics::bgColor << "  " << Aesthetics::reset_color;
		}

		out << '\n';
	}

	return out;
}

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

	bool isDead() const { return mHealth <= 0; }
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

	int getModifiedHealth() const { return mModification.getHealth(); }
	int getModifiedDamage() const { return mModification.getDamage(); }

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

class Creature : public Entity, public CreatureBase
{
public:
	Creature(Map& map, const Point& position, char symbol, Aesthetics::Color color, int health, int damage)
		: Entity{ map, position, symbol, color }, CreatureBase{ health, damage }
	{

	}

	Creature(Map& map, const Point& position, char symbol, Aesthetics::Color color, CreatureBase base)
		: Entity{ map, position, symbol, color }, CreatureBase{ base }
	{

	}

	std::unique_ptr<Entity> clone() const override
	{
		return std::make_unique<Creature>(*this);
	}
};

class Item
{
public:
	virtual const char* getName() const = 0;
	virtual const Effect& getEffect() const = 0;
	virtual void description() const = 0;

	void useOn(CreatureBase& on) const
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

	inline static constexpr std::array descriptions{
		"A wooden stick, held by many legendary warriors in the past.",
		"A boring dagger.",
		"A long sword made of iron and love.",
		"A musket which can hurt things or something.",
	};
	static_assert(descriptions.size() == max_types);

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
	Weapon() = default;

	Weapon(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return damages[mType]; }
	int getDamage() const { return getEffect().getModifiedHealth() * -1; }

	void description() const override
	{
		std::cout << descriptions[mType] << " Deals " << getDamage() << " damage\n\n";
	}
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

	inline static constexpr std::array names{ "potion of healing", "potion of pain and suffering", "potion of strength", "potion of weakness" };
	static_assert(names.size() == max_types);

	inline static constexpr std::array descriptions{
		"A red potion and not green for some reason. Increases health by ",
		"A green potion and not red for some reason. Reduces health by ",
		"A potion that you can't see the color of because it's in an opaque bottle. Increases damage by ",
		"It's just beer. Reduces damage by ",
	};
	static_assert(descriptions.size() == max_types);

	inline static const std::array effects{
		Effect::modifiedHealth(5),
		Effect::modifiedHealth(-5),
		Effect::modifiedDamage(2),
		Effect::modifiedDamage(-2),
	};
	static_assert(effects.size() == max_types);

	inline static constexpr std::array applyOnUser{ true, false, true, false };

private:
	Type mType{};

public:
	Potion() = default;

	Potion(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return effects[mType]; }

	void description() const override
	{
		std::cout << descriptions[mType] << (getEffect().getModifiedHealth() == 0 ? getEffect().getModifiedDamage() : getEffect().getModifiedHealth()) << "\n\n";
	}
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

	inline static constexpr std::array descriptions{
		"Yummy bread.",
		"Yummier bread.",
		"Organic flesh.",
		"A sandwich with an odd choice of ingredients.",
	};
	static_assert(descriptions.size() == max_types);

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
	Food() = default;

	Food(Type type)
		: mType{ type }
	{

	}

	const char* getName() const override { return names[mType]; }
	const Effect& getEffect() const override { return heals[mType]; }
	int getHealingAmount() const { return heals[mType].getModifiedHealth(); }

	void description() const override
	{
		std::cout << descriptions[mType] << " Heals you by " << getHealingAmount() << "\n\n";
	}
};

class Player : public Creature
{
private:
	std::vector<Weapon> mWeapons{};
	std::vector<Potion> mPotions{};
	std::vector<Food> mFood{};
	std::size_t mEquippedWeaponIndex{};

public:
	Player(Map& map, const Point& position)
		: Creature{ map, position, 'P', Aesthetics::yellow, 10, 1 }
	{

	}

	std::unique_ptr<Entity> clone() const override
	{
		return std::make_unique<Player>(*this);
	}

	const std::vector<Weapon>& getWeapons() const { return mWeapons; }
	const std::vector<Potion>& getPotions() const { return mPotions; }
	const std::vector<Food>& getFood() const { return mFood; }

	const Weapon& getEquippedWeapon() const { return mWeapons[mEquippedWeaponIndex]; }
	void equipWeaponAtIndex(int index) { mEquippedWeaponIndex = static_cast<std::size_t>(index); }

	void addItem(const Item& item)
	{
		if (const Weapon* weapon{ dynamic_cast<const Weapon*>(&item) })
			mWeapons.push_back(*weapon);
		else if (const Potion* potion{ dynamic_cast<const Potion*>(&item) })
			mPotions.push_back(*potion);
		else if (const Food* food{ dynamic_cast<const Food*>(&item) })
			mFood.push_back(*food);
	}
};

using namespace std::string_literals;

class Enemy : public Creature
{
public:
	enum Type
	{
		goblin,
		wizard,
		knight,

		max_types,
	};

	inline static const std::array names{ "goblin"s, "wizard"s, "knight"s };
	static_assert(names.size() == max_types);

	constexpr static std::array symbols{ 'G', 'W', 'K' };
	static_assert(symbols.size() == max_types);

	inline static const std::array stats{ CreatureBase{ 5, 1 }, CreatureBase{ 10, 2 }, CreatureBase{ 20, 4 } };
	static_assert(stats.size() == max_types);

private:
	Type mType{};
	std::string mName{};

public:
	Enemy(Map& map, const Point& position, Type type)
		: mType{ type }, mName{ names[type] }, Creature{ map, position, symbols[type], Aesthetics::red, stats[type] }
	{

	}

	std::unique_ptr<Entity> clone() const override
	{
		return std::make_unique<Enemy>(*this);
	}

	const std::string& getName() const { return mName; }
};

class Wall : public Entity
{
public:
	Wall(Map& map, const Point& position)
		: Entity{ map, position, '#', Aesthetics::magenta }
	{

	}

	std::unique_ptr<Entity> clone() const override
	{
		return std::make_unique<Wall>(*this);
	}
};

class Block
{
private:
	std::vector<std::vector<std::unique_ptr<Entity>>> mBlock{};
	Point mStart{};

public:
	Block(const Entity& sample, const Point& start, const Point& end, const Point& gap = {})
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

	Entity& getEntity(const Point& position)
	{
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

template <typename T>
std::vector<const Item*> createCategory(const std::vector<T>& items)
{
	if (items.empty())
		return;

	std::vector<const Item*> category{};

	category.reserve(items.size());
	for (const auto& t : items)
		category.push_back(&t);

	return category;
}

class Inventory
{
private:
	inline static std::vector<std::vector<const Item*>> mCategories{};
	inline static std::vector<std::string> mNames{};
	inline static Point mIndicator{};
	constexpr static int mNameWidth{ 20 };
	constexpr static int mIndicatorWidth{ 10 };
	constexpr static int mCategoryWidth{ mNameWidth + mIndicatorWidth };
	inline static bool mCleared{ true };

	Inventory() = default;

	template <typename T>
	static void addCategory(const std::string& name, const std::vector<T>& items)
	{
		mNames.push_back(name);
		mCategories.push_back(createCategory(items));
	}

	static void displayCategoryNames(const std::vector<std::string>& names)
	{
		std::cout << Aesthetics::clear << '\n';
		for (const auto& name : names)
			std::cout << std::setw(mCategoryWidth) << name;
		std::cout << "\n\n";
	}

	static void displayItemAt(std::vector<std::vector<const Item*>> categories, std::size_t x, std::size_t y, const Player& player)
	{
		if (categories[x].size() <= y)
			return;

		const Item* item{ categories[x][y] };
		int width{ mNameWidth };
		if (item == &player.getEquippedWeapon())
		{
			std::cout << "* ";
			width -= 2;
		}

		std::cout << std::setw(width) << item->getName() << std::setw(mIndicatorWidth) << (mIndicator.x == x && mIndicator.y == y ? " <" : " .");
	}

	static void displayItems(std::vector<std::vector<const Item*>> categories, const Player& player)
	{
		std::size_t height{};
		for (const auto& items : categories)
			height = std::max(height, items.size());

		for (std::size_t y{}; y < height; ++y)
		{
			for (std::size_t x{}; x < mCategories.size(); ++x)
			{
				displayItemAt(categories, x, y, player);
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

	static void displayInventory(const std::vector<std::string>& names, std::vector<std::vector<const Item*>> categories, const Player& player)
	{
		if (mCategories.empty())
			std::cout << "Your inventory is empty\n\n";
		else
		{
			displayCategoryNames(names);
			displayItems(categories, player);
		}
	}

	static void displayItemInfo(const Item* item, const Player& player)
	{
		if (item)
			item->description();

		if (item == &player.getEquippedWeapon())
			std::cout << "Equipped\n\n";
		else if (dynamic_cast<const Weapon*>(item))
			std::cout << "Press 'E' to equip\n\n";
	}

	static void clampIndicator()
	{
		// categories.size() - 1 might equal -1 if the size is 0 but if it is 0 then we won't really need to show the indicator anyway
		mIndicator.x = std::clamp(mIndicator.x, 0, static_cast<int>(mCategories.size()) - 1);
		mIndicator.y = std::clamp(mIndicator.y, 0, static_cast<int>(mCategories[static_cast<std::size_t>(mIndicator.x)].size()) - 1);
	}

	static void moveIndicator(char direction)
	{
		switch (direction)
		{
		case 'w': --mIndicator.y; break;
		case 'a': --mIndicator.x; break;
		case 's': ++mIndicator.y; break;
		case 'd': ++mIndicator.x; break;
		default: return;
		}

		clampIndicator();
	}

	static void addCategories(const Player& player)
	{
		addCategory("Weapons", player.getWeapons());
		addCategory("Potions", player.getPotions());
		addCategory("Food", player.getFood());
	}

	static void clear()
	{
		mCategories.clear();
		mNames.clear();
		mIndicator = Point{};

		mCleared = true;
	}

public:
	Inventory(const Inventory&) = delete;
	Inventory& operator=(const Inventory&) = delete;

	static const Item* getIndicatedItem(const std::vector<std::vector<const Item*>>& categories, const Point& indicator)
	{
		return categories[static_cast<std::size_t>(mIndicator.x)][static_cast<std::size_t>(mIndicator.y)];
	}

	static void display(const std::vector<std::string>& names, const std::vector<std::vector<const Item*>>& categories, const Point& indicator, const Player& player)
	{
		const Item* indicatedItem{ getIndicatedItem(categories, indicator)};

		displayInventory(names, categories, player);
		displayItemInfo(indicatedItem, player);
	}

	static void inventory(Player& player)
	{
		if (mCleared) addCategories(player);
		mCleared = false;

		while (true)
		{
			display(mNames, mCategories, mIndicator, player);

			std::cout << "( WASD to move cursor, 'q' to quit inventory )";
			char c{ getInput() };

			if (c == 'q')
				break;
			else if (c == 'e' && dynamic_cast<const Weapon*>(mCategories[static_cast<std::size_t>(mIndicator.x)][static_cast<std::size_t>(mIndicator.y)]))
				player.equipWeaponAtIndex(mIndicator.y);

			moveIndicator(c);
		}

		std::cout << '\n';

		clear();
	}
};

struct Option
{
	std::string name{};
	std::function<void(Player& player, Enemy& enemy)> action{};
};

void attack(Player& player, Enemy& enemy)
{
	player.getEquippedWeapon().useOn(enemy);
}

void usePotion(Player& player, Enemy& enemy)
{
	Point indicator{};

	while (true)
	{
		Inventory::display({ "Potions" }, { createCategory(player.getPotions()) }, indicator, player);

	}
}

void eatFood(Player& player, Enemy& enemy)
{

}

void tryToFlee(Player& player, Enemy& enemy)
{

}

void displayFight(const Player& player, const Enemy& enemy)
{
	int enemyHealth{ enemy.getHealth() < 0 ? 0 : enemy.getHealth() };
	int playerHealth{ player.getHealth() < 0 ? 0 : player.getHealth() };

	std::cout << "\t" << std::setw(10) << enemy.getSymbol() << player.getSymbol() << "\n\n";
	std::cout << "\t" << "hp: " << std::setw(6) << enemyHealth << "hp: " << playerHealth << "\n\n";
}

void fight(Player& player, Enemy& enemy)
{
	std::cout << Aesthetics::clear << '\n';
	displayFight(player, enemy);

	std::cout << "A fight has started between you and the " << enemy.getName() << '\n';

	std::array options{
		Option{ "attack", attack },
		Option{ "use potion", usePotion },
		Option{ "eat food", eatFood },
		Option{ "try to flee", tryToFlee },
	};

	int indicator{};
	
	while (true)
	{
		for (std::size_t i{}; i < options.size(); ++i)
			std::cout << (static_cast<std::size_t>(indicator) == i ? "> " : ". ") << std::setw(15) << options[i].name;
		std::cout << "\n\n( A/D to move between options, 'E' to select )\n";

		char c{ getInput() };

		if (c == 'e')
		{
			options[static_cast<std::size_t>(indicator)].action(player, enemy);
			if (!enemy.isDead())
				player.setHealth(player.getHealth() - enemy.getDamage());

			std::cout << Aesthetics::clear << '\n';
			displayFight(player, enemy);

			std::cout << "You dealt " << player.getEquippedWeapon().getEffect().getModifiedHealth() * -1 << " damage to the " << enemy.getName() << '\n';
			if (enemy.isDead())
			{
				std::cout << "They are now just a soulless husk\n";
				break;
			}

			std::cout << "They dealt " << enemy.getDamage() << " damage to you\n";
			if (player.isDead())
				break;

			continue;
		}

		switch (c)
		{
		case 'a': --indicator; break;
		case 'd': ++indicator; break;
		default: continue;
		}

		indicator = std::clamp(indicator, 0, static_cast<int>(options.size()) - 1);

		std::cout << Aesthetics::clear << '\n';
		displayFight(player, enemy);
	}
}

int main()
{
	Map map{ 10, 10 };

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
	Block block{ Wall{ map, Point{} }, Point{2, 8}, Point{1, 7} };
	Block block2{ Wall{ map, Point{} }, Point{ 6, 6 }, Point{ 3, 8 } };
	Block block3{ Wall{ map, Point{} }, Point{ 8, 0 }, Point{ 9, 1 } };

	Entity e2{ e };
	e.move(Directions::left);

	std::cout << map << '\n';

	//e2 = p;

	//std::cout << map << '\n';

	Block block4{ Entity{ map, Point{}, 'E' }, Point{ 0, 0 }, Point{ 2, 2 }, Point{ 1, 1 } };

	std::cout << map << '\n';

	Map map2{ 20, 20 };

	Block block5{ Entity{ map2, Point{}, 'E' }, Point{ 0, 0 }, Point{ 10, 10 }, Point{ 1, 1 } };

	std::cout << map2 << '\n';

	Block block6{ Entity{ map2, Point{}, 'E' }, Point{ 1, 1 }, Point{ 9, 11 }, Point{ 1, 1 } };

	std::cout << map2 << '\n';

	std::cout << block6.getEntity(Point{ 1, 1 }).getPosition() << '\n';
	//std::cout << block6.getEntity(Point{ 2, 2 }).getPosition() << '\n'; SHOULD ASSERT OUT
	//std::cout << block6.getEntity(Point{ -1, -1 }).getPosition() << '\n'; SHOULD ASSERT OUT

	Enemy enemy{ map, Point{ 0, 7 }, Enemy::goblin };
	Enemy enemy2{ map, Point{ 8, 8 }, Enemy::knight };

	Weapon weapon{ Weapon::sword };
	p.addItem(weapon);
	p.addItem(Weapon{ Weapon::musket });
	p.addItem(Weapon{ Weapon::dagger });
	p.addItem(Potion{ Potion::healing });
	p.addItem(Potion{ Potion::healing });
	p.equipWeaponAtIndex(0);

	std::cout << std::left;

	while (!p.isDead())
	{
		std::cout << "Enter a key: ";
		char c{ getInput() };
		std::cout << Aesthetics::clear << '\n';

		Point dir{};
		switch (c)
		{
		case 'w': dir = Directions::up; break;
		case 'a': dir = Directions::left; break;
		case 's': dir = Directions::down; break;
		case 'd': dir = Directions::right; break;
		case 'i': Inventory::inventory(p);
		}

		Entity* entity{ map.getEntity(p.getPosition() + dir) };
		if (!entity)
			p.move(dir);
		else if (Enemy* opponent{ dynamic_cast<Enemy*>(entity) })
		{
			std::cout << map << '\n';
			fight(p, *opponent);

			if (opponent->isDead())
				opponent->deactivate();

			continue;
		}

		std::cout << map << '\n';
	}

	std::cout << "You lost\n";

	return 0;
}
