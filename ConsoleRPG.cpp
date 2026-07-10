#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>
#include <conio.h>
#include "Random.h"

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

namespace Keybinds
{
	struct Keybind
	{
		std::vector<char> keys{};
	};

	const Keybind up{ { 'W', 'w' } };
	const Keybind down{ { 'S', 's' } };
	const Keybind left{ { 'A', 'a' } };
	const Keybind right{ { 'D', 'd' } };
	const Keybind inventory{ { 'I', 'i' } };
	const Keybind select{ { 'E', 'e' } };
	const Keybind agree{ { 'Y', 'y' } };
	const Keybind disagree{ { 'N', 'n' } };
	const Keybind quit{ { 'Q', 'q' } };

	const std::vector<Keybind> movementKeybinds{ up, left, down, right };
}

std::ostream& operator<<(std::ostream& out, const Keybinds::Keybind& keybind)
{
	return out << keybind.keys[0];
}

std::ostream& operator<<(std::ostream& out, const std::vector<Keybinds::Keybind>& keybindCombination)
{
	for (const auto& keybind : keybindCombination)
		out << keybind;
	return out;
}

bool operator==(char c, const Keybinds::Keybind& keybind)
{
	return std::find(keybind.keys.begin(), keybind.keys.end(), c) != keybind.keys.end();
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
	if (n == 0)
		return 0;

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
	Map* mMap;
	Point mPosition{};
	char mSymbol{};
	Aesthetics::Color mColor{};
	bool active{ true };

	void removeFromMap() { mMap->removeEntity(mPosition, this); }

public:
	Entity(Map* map, const Point& position, char symbol, Aesthetics::Color color = Aesthetics::white)
		: mMap{ map }, mPosition{ position }, mSymbol{ symbol }, mColor{ color }
	{
		mMap->addEntity(mPosition, this);
	}

	Entity(const Entity& other)
		: Entity{ other.mMap, other.mPosition, other.mSymbol, other.mColor }
	{
		
	}

	virtual ~Entity()
	{
		removeFromMap();
	}

	Map* getMap() const { return mMap; }
	const Point& getPosition() const { return mPosition; }
	char getSymbol() const { return mSymbol; }
	Aesthetics::Color getColor() const { return mColor; }

	void setMap(Map* map) { mMap = map; }

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactivate() { active = false; }

	void moveTo(const Point& to)
	{
		if (!mMap->isPositionInBounds(to))
			return;

		removeFromMap(); // remove first to account for the scenario when mPosition and to is same ( if we added first removeEntity would remove the instance just added as well )
		mMap->addEntity(to, this);
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
protected:
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
	void setHealth(int health)
	{
		mHealth = health;
		if (mHealth < 0)
			mHealth = 0;
	}

	int getDamage() const { return mDamage; }
	void setDamage(int damage)
	{
		mDamage = damage;
		if (mDamage < 0)
			mDamage = 0;
	}

	void damage(int amount) { setHealth(mHealth - amount); }
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
	Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, int health, int damage)
		: Entity{ map, position, symbol, color }, CreatureBase{ health, damage }
	{

	}

	Creature(Map* map, const Point& position, char symbol, Aesthetics::Color color, CreatureBase base)
		: Entity{ map, position, symbol, color }, CreatureBase{ base }
	{

	}
};

class Item
{
public:
	virtual const char* getName() const = 0;
	virtual const Effect& getEffect() const = 0;
	virtual void description() const = 0;

	virtual ~Item() = default;

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
		std::cout << descriptions[mType] << " Deals " << getDamage() << " damage\n";
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
		std::cout << descriptions[mType] << (getEffect().getModifiedHealth() == 0 ? getEffect().getModifiedDamage() : getEffect().getModifiedHealth()) << "\n";
	}

	bool isAppliedOnUser() const { return applyOnUser[mType]; }
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
		Effect::modifiedHealth(5),
		Effect::modifiedHealth(10),
		Effect::modifiedHealth(20),
		Effect::modifiedHealth(40),
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
		std::cout << descriptions[mType] << " Heals you by " << getHealingAmount() << "\n";
	}
};

class Player : public Creature
{
public:
	struct Inventory
	{
		std::vector<Weapon> weapons{};
		std::vector<Potion> potions{};
		std::vector<Food> food{};
		std::size_t equippedWeaponIndex{};
	};

private:
	Inventory mInventory{};
	int mXp{};
	int mRequiredXp{ 50 };
	int mLevel{};

public:
	Player(Map* map, const Point& position)
		: Creature{ map, position, 'P', Aesthetics::yellow, 10, 0 }
	{

	}

	const std::vector<Weapon>& getWeapons() const { return mInventory.weapons; }
	const std::vector<Potion>& getPotions() const { return mInventory.potions; }
	const std::vector<Food>& getFood() const { return mInventory.food; }

	const Weapon* getEquippedWeapon() const { return getWeapons().empty() ? nullptr : &getWeapons()[mInventory.equippedWeaponIndex]; }
	void equipWeaponAtIndex(int index) { mInventory.equippedWeaponIndex = static_cast<std::size_t>(index); }

	const Inventory& getInventory() const { return mInventory; }
	void setInventory(const Inventory& inventory) { mInventory = inventory; }

	int getLevel() const { return mLevel; }

	void addItem(const Item& item)
	{
		if (const Weapon* weapon{ dynamic_cast<const Weapon*>(&item) })
			mInventory.weapons.push_back(*weapon);
		else if (const Potion* potion{ dynamic_cast<const Potion*>(&item) })
			mInventory.potions.push_back(*potion);
		else if (const Food* food{ dynamic_cast<const Food*>(&item) })
			mInventory.food.push_back(*food);
	}

	void removeItem(const Item& item)
	{
		if (const Weapon * weapon{dynamic_cast<const Weapon*>(&item)})
			std::erase_if(mInventory.weapons, [&](const auto& weapon) { return &weapon == &item; });
		else if (const Potion* potion{ dynamic_cast<const Potion*>(&item) })
			std::erase_if(mInventory.potions, [&](const auto& potion) { return &potion == &item; });
		else if (const Food* food{ dynamic_cast<const Food*>(&item) })
			std::erase_if(mInventory.food, [&](const auto& food) { return &food == &item; });
	}

	void addXp(int amount)
	{
		mXp += amount;
		while (mXp >= mRequiredXp)
		{
			mXp -= mRequiredXp;
			mRequiredXp *= 2;
			++mLevel;
			++mDamage;

			std::cout << "You leveled up! You are now level " << mLevel << ". Your damage has been increased by 1\n";
		}
	}
};

using namespace std::string_literals;

class Enemy : public Creature
{
public:
	enum Type
	{
		goblin,
		exile,
		knight,
		ogre,

		max_types,
	};

	inline static const std::array names{ "goblin"s, "exile"s, "knight"s, "ogre"s };
	static_assert(names.size() == max_types);

	constexpr static std::array symbols{ 'G', 'E', 'K', 'O' };
	static_assert(symbols.size() == max_types);

	inline static const std::array stats{ CreatureBase{ 5, 1 }, CreatureBase{ 10, 2 }, CreatureBase{ 20, 4 }, CreatureBase{ 40, 8 } };
	static_assert(stats.size() == max_types);

	constexpr static std::array tiers{ 1, 2, 3, 4 };
	static_assert(tiers.size() == max_types);

	constexpr static int baseXp{ 10 };

private:
	Type mType{};
	std::string mName{};

public:
	Enemy(Map* map, const Point& position, Type type)
		: mType{ type }, mName{ names[type] }, Creature{ map, position, symbols[type], Aesthetics::red, stats[type] }
	{

	}

	const std::string& getName() const { return mName; }
	int getTier() const { return tiers[mType]; }
	int getXp() const { return baseXp * getTier(); }
};

class Wall : public Entity
{
public:
	Wall(Map* map, const Point& position)
		: Entity{ map, position, '#', Aesthetics::magenta }
	{
		
	}
};

class DoorPair;

class Door : public Entity
{
private:
	const DoorPair* mDoorPair;

public:
	Door(Map* map, const Point& position, const DoorPair* doorPair)
		: Entity{ map, position, '*', Aesthetics::blue }, mDoorPair{ doorPair }
	{

	}

	const DoorPair* getDoorPair() const { return mDoorPair; }
};

class DoorPair
{
private:
	Door first;
	Door second;

public:
	DoorPair(Map* firstMap, const Point& firstPosition, Map* secondMap, const Point& secondPosition)
		: first{ firstMap, firstPosition, this }, second{ secondMap, secondPosition, this }
	{

	}

	DoorPair(const DoorPair& other)
		: first{ other.first.getMap(), other.first.getPosition(), this }, second{ other.second.getMap(), other.second.getPosition(), this }
	{
	
	}

	const Door& getFirst() const { return first; }
	const Door& getSecond() const { return second; }
};

class Block
{
private:
	std::vector<std::vector<std::shared_ptr<Entity>>> mBlock{}; // shared so that block can be used in vectors
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
				ptr = std::make_shared<Entity>(sample);
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

void waitForInput()
{
	std::cout << "\nPress anything to continue\n";
	getInput();
}

struct Category
{
	std::vector<const Item*> items;
	std::string name;
};

template <typename T>
Category createCategory(const std::vector<T>& items, std::string_view name)
{
	Category category{};

	category.name = name;
	category.items.reserve(items.size());
	for (const auto& t : items)
		category.items.push_back(&t);

	return category;
}

namespace Inventory
{
	constexpr static int nameWidth{ 20 };
	constexpr static int indicatorWidth{ 10 };
	constexpr static int categoryWidth{ nameWidth + indicatorWidth };

	template <typename T>
	void addCategory(std::vector<Category>& categories, std::string_view name, const std::vector<T>& items)
	{
		if (items.empty())
			return;

		categories.push_back(createCategory(items, name));
	}

	Category getWeaponCategory(const Player& player) { return createCategory(player.getWeapons(), "Weapons"); }
	Category getPotionCategory(const Player& player) { return createCategory(player.getPotions(), "Potions"); }
	Category getFoodCategory(const Player& player) { return createCategory(player.getFood(), "Food"); }

	void displayCategoryNames(const std::vector<Category>& categories)
	{
		for (const auto& category : categories)
			std::cout << std::setw(categoryWidth) << category.name;
		std::cout << "\n\n";
	}

	void displayItemAt(const std::vector<Category>& categories, const Point& indicator, std::size_t x, std::size_t y, const Player& player)
	{
		if (categories[x].items.size() <= y)
		{
			std::cout << std::setw(nameWidth + indicatorWidth) << " ";
			return;
		}

		const Item* item{ categories[x].items[y] };
		int width{ nameWidth };
		if (item == player.getEquippedWeapon())
		{
			std::cout << "* ";
			width -= 2;
		}

		std::cout << std::setw(width) << item->getName() << std::setw(indicatorWidth) << (indicator.x == x && indicator.y == y ? " <" : " .");
	}

	void displayItems(const std::vector<Category>& categories, const Point& indicator, const Player& player)
	{
		std::size_t height{};
		for (const auto& category : categories)
			height = std::max(height, category.items.size());

		for (std::size_t y{}; y < height; ++y)
		{
			for (std::size_t x{}; x < categories.size(); ++x)
			{
				displayItemAt(categories, indicator, x, y, player);
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

	void displayInventory(const std::vector<Category>& categories, const Point& indicator, const Player& player)
	{
		displayCategoryNames(categories);
		displayItems(categories, indicator, player);
	}

	void displayItemInfo(const Item* item, const Player& player)
	{
		if (item)
			item->description();

		if (item == player.getEquippedWeapon())
			std::cout << "\nEquipped\n";
		else if (dynamic_cast<const Weapon*>(item))
			std::cout << "\nPress " << Keybinds::select << " to equip\n";
	}

	void clampIndicator(Point& indicator, const std::vector<Category>& categories)
	{
		// categories.size() - 1 might equal -1 if the size is 0 but if it is 0 then we won't really need to show the indicator anyway
		indicator.x = std::clamp(indicator.x, 0, static_cast<int>(categories.size()) - 1);
		indicator.y = std::clamp(indicator.y, 0, static_cast<int>(categories[static_cast<std::size_t>(indicator.x)].items.size()) - 1);
	}

	void addCategories(std::vector<Category>& categories, const Player& player)
	{
		addCategory(categories, "Weapons", player.getWeapons());
		addCategory(categories, "Potions", player.getPotions());
		addCategory(categories, "Food", player.getFood());
	}

	const Item* getIndicatedItem(const std::vector<Category>& categories, const Point& indicator)
	{
		return categories[static_cast<std::size_t>(indicator.x)].items[static_cast<std::size_t>(indicator.y)];
	}

	void display(const std::vector<Category>& categories, const Point& indicator, const Player& player)
	{
		displayInventory(categories, indicator, player);
		displayItemInfo(getIndicatedItem(categories, indicator), player);
	}

	void moveIndicator(Point& indicator, const std::vector<Category>& categories, char direction)
	{
		if (direction == Keybinds::up) --indicator.y;
		else if (direction == Keybinds::down) ++indicator.y;
		else if (direction == Keybinds::left) --indicator.x;
		else if (direction == Keybinds::right) ++indicator.x;
		else return;

		clampIndicator(indicator, categories);
	}

	void inventory(Player& player)
	{
		Point indicator{};
		std::vector<Category> categories;
		addCategories(categories, player);

		while (true)
		{
			std::cout << Aesthetics::clear << '\n';
			if (categories.empty())
				std::cout << "Your inventory is empty\n";
			else
				display(categories, indicator, player);

			std::cout << "\n( " << Keybinds::movementKeybinds << " to move cursor, " << Keybinds::quit << " to quit inventory )\n";
			char c{ getInput() };

			if (c == Keybinds::quit)
				break;

			if (categories.empty())
				continue;

			if (c == Keybinds::select && dynamic_cast<const Weapon*>(getIndicatedItem(categories, indicator)))
				player.equipWeaponAtIndex(indicator.y);
			else
				moveIndicator(indicator, categories, c);
		}

		std::cout << '\n';
	}
};

namespace Loot
{
	const Weapon dagger{ Weapon::dagger };
	const Weapon sword{ Weapon::sword };
	const Weapon musket{ Weapon::musket };
	const Potion healing{ Potion::healing };
	const Potion pain{ Potion::pain };
	const Potion strength{ Potion::strength };
	const Potion weakness{ Potion::weakness };
	const Food bread{ Food::bread };
	const Food bread_two{ Food::bread_two };
	const Food beef{ Food::beef };
	const Food sandvich{ Food::sandvich };

	struct Loot
	{
		const Item* item{};
		int weight;
		bool eligible{ true };
	};

	std::vector<std::vector<Loot>> lootTable{
		{ // Tier 1
			Loot{ &dagger, 3 },
			Loot{ &healing, 1 },
			Loot{ &pain, 1 },
			Loot{ &bread, 1 },
		},
		{ // Tier 2
			Loot{ &sword, 3 },
			Loot{ &healing, 1 },
			Loot{ &bread_two, 2 },
		},
		{ // Tier 3
			Loot{ &musket, 3 },
			Loot{ &strength, 1 },
			Loot{ &weakness, 2 },
			Loot{ &beef, 2 },
		},
		{ // Tier 4
			Loot{ &musket, 2 },
			Loot{ &strength, 2 },
			Loot{ &weakness, 2 },
			Loot{ &sandvich, 2 },
		},
	};

	bool isTierEligible(std::size_t index)
	{
		for (const auto& loot : lootTable[index])
			if (loot.eligible)
				return true;
		return false;
	}

	void resetTierEligibility(std::size_t index)
	{
		for (auto& loot : lootTable[index])
			loot.eligible = true;
	}

	int totalWeightOfTier(std::size_t index)
	{
		int totalWeight{};
		for (const auto& loot : lootTable[index])
			totalWeight += (loot.weight * static_cast<int>(loot.eligible));
		return totalWeight;
	}

	const Item* getRandomLoot(int tier)
	{
		assert((tier > 0 && tier <= ssize(lootTable)) && "Invalid tier for loot table");
		std::size_t stier{ static_cast<std::size_t>(tier - 1) };

		if (isTierEligible(stier) == false)
			resetTierEligibility(stier);

		int totalTierWeight{ totalWeightOfTier(stier) };
		int chance{ Random::get(1, totalTierWeight) };
		int accumulativeWeight{};
		for (auto& loot : lootTable[stier])
		{
			if (!loot.eligible)
				continue;

			accumulativeWeight += loot.weight;
			if (chance <= accumulativeWeight)
			{
				loot.eligible = false;
				return loot.item;
			}
		}

		return nullptr;
	}
}

namespace Fighting
{
	constexpr int fleeChance{ 3 };
	constexpr int displayWidth{ 10 };
	constexpr int exit{ 1 };
	constexpr int stay{ 0 };

	struct FightState
	{
		Player& player;
		Enemy& enemy;
		std::function<void()> announce;
		bool finished{ false };
		bool playersTurn{ true };
	};

	struct Option
	{
		std::string name{};
		std::function<void(FightState&)> action{};
	};

	void attack(FightState& state)
	{
		state.enemy.damage(state.player.getDamage());

		if (state.player.getEquippedWeapon())
		{
			state.player.getEquippedWeapon()->useOn(state.enemy);
			int totalDamage{ state.player.getDamage() + state.player.getEquippedWeapon()->getDamage() };
			state.announce = [totalDamage, &state]() { std::cout << "You dealt " << totalDamage << " damage to the " << state.enemy.getName() << '\n'; };
		}
		else
			state.announce = [&]() { std::cout << "You dealt " << state.player.getDamage() << " damage to the " << state.enemy.getName() << '\n'; };
	}

	void displayItemUsage(FightState& state, const Category& category, const Point& indicator)
	{
		std::cout << Aesthetics::clear << "Choose an item to use:\n\n";
		Inventory::display({ category }, indicator, state.player);
		std::cout << "\n( " << Keybinds::up << '/' << Keybinds::down << " to move between items, " << Keybinds::select << " to select, " << Keybinds::quit << " to cancel )\n";
	}

	int processInputForItemUsage(FightState& state, const Category& category, Point& indicator, const std::function<void(FightState&, const Item*)>& onItemSelect)
	{
		char input{ getInput() };

		if (input == Keybinds::select)
		{
			const Item* item{ Inventory::getIndicatedItem({ category }, indicator) };
			if (item)
			{
				onItemSelect(state, item);
				//std::cout << item << '\n';
				//std::cout << Inventory::getIndicatedItem({ category }, indicator + Point{ 0, 1 }) << '\n';
				state.player.removeItem(*item);
				return exit;
			}

			return stay;
		}
		else if (input == Keybinds::quit)
		{
			state.playersTurn = true;
			return exit;
		}
		else
		{
			Inventory::moveIndicator(indicator, { category }, input);
			return stay;
		}
	}

	void useItem(FightState& state, const Category& category, const std::function<void(FightState&)>& onItemAbsence, const std::function<void(FightState&, const Item*)>& onItemSelect)
	{
		if (category.items.empty())
		{
			onItemAbsence(state);
			return;
		}

		Point indicator{};

		do {
			displayItemUsage(state, category, indicator);
		} while (processInputForItemUsage(state, category, indicator, onItemSelect) == 0);
	}

	void onPotionAbsence(FightState& state)
	{
		state.announce = []() { std::cout << "You have no potions\n"; };
		state.playersTurn = true;
	}

	void selectPotion(FightState& state, const Item* item)
	{
		const Potion* potion{ dynamic_cast<const Potion*>(item) };
		if (potion)
		{
			potion->isAppliedOnUser() ? potion->useOn(state.player) : potion->useOn(state.enemy);
			state.announce = [p{ *potion }]() { std::cout << "You used a " << p.getName() << " on " << (p.isAppliedOnUser() ? "yourself " : "the enemy ") << '\n'; }; // it still works for some reason when I make it &?
		}
	}

	void usePotion(FightState& state)
	{
		useItem(state, Inventory::getPotionCategory(state.player), onPotionAbsence, selectPotion);
	}

	void onFoodAbsence(FightState& state)
	{
		state.announce = []() { std::cout << "You have no food\n"; };
		state.playersTurn = true;
	}

	void selectFood(FightState& state, const Item* item)
	{
		const Food* food{ dynamic_cast<const Food*>(item) };
		if (food)
		{
			food->useOn(state.player);
			state.announce = [f{ *food }]() { std::cout << "You ate a " << f.getName() << '\n'; };
			//state.announce = [=]() { std::cout << "You ate a " << food->getName() << '\n'; };//food << '\n'; };
		}
	}

	void eatFood(FightState& state)
	{
		useItem(state, Inventory::getFoodCategory(state.player), onFoodAbsence, selectFood);
	}

	void tryToFlee(FightState& state)
	{
		if (Random::get(1, fleeChance) == 1)
		{
			state.finished = true;
			state.announce = []() { std::cout << "You successfully fled like a coward\n"; };
		}
		else
			state.announce = []() { std::cout << "You couldn't flee\n"; };
	}

	const std::array options{
		Option{ "attack", attack },
		Option{ "use potion", usePotion },
		Option{ "eat food", eatFood },
		Option{ "try to flee", tryToFlee },
	};

	void displayFight(const Player& player, const Enemy& enemy)
	{
		int enemyHealth{ enemy.getHealth() < 0 ? 0 : enemy.getHealth() };
		int playerHealth{ player.getHealth() < 0 ? 0 : player.getHealth() };

		std::cout << "\t" << std::setw(displayWidth) << enemy.getSymbol() << player.getSymbol() << "\n\n";
		std::cout << "\t" << "hp: " << std::setw(displayWidth - 4) << enemyHealth << "hp: " << playerHealth << "\n\n"; // 4 is the length of "hp: "
	}

	void displayState(const FightState& state)
	{
		std::cout << Aesthetics::clear << '\n';
		displayFight(state.player, state.enemy);

		state.announce();
	}

	void displayOptions(int indicator)
	{
		for (std::size_t i{}; i < options.size(); ++i)
			std::cout << (static_cast<std::size_t>(indicator) == i ? "> " : ". ") << std::setw(15) << options[i].name;
	}

	void doAction(FightState& state, int indicator)
	{
		state.announce = []() { std::cout << "...\n"; }; // in case action doesn't set an announce ( me ) reset announce to avoid showing the previous one after the screen is cleared in the next loop iteration ( AI )
		state.playersTurn = false; // do it before in case the action modifies playersTurn itself
		options[static_cast<std::size_t>(indicator)].action(state);
	}

	void moveIndicator(int& indicator, char direction)
	{
		if (direction == Keybinds::left) --indicator;
		else if (direction == Keybinds::right) ++indicator;

		indicator = std::clamp(indicator, 0, static_cast<int>(options.size()) - 1);
	}

	void enemyAttack(FightState& state)
	{
		state.player.setHealth(state.player.getHealth() - state.enemy.getDamage());
		state.announce = [&]() { std::cout << "They dealt " << state.enemy.getDamage() << " damage to you\n"; };
		state.playersTurn = true;
	}

	void playersTurn(FightState& state, int& indicator)
	{
		displayOptions(indicator);
		std::cout << "\n\n( " << Keybinds::left << '/' << Keybinds::right << " to move between options, " << Keybinds::select << " to select )\n";

		char c{ getInput() };

		if (c == Keybinds::select)
			doAction(state, indicator);
		else
			moveIndicator(indicator, c);
	}

	void enemysTurn(FightState& state)
	{
		waitForInput();
		enemyAttack(state);
	}

	void equipWeaponLoot(const FightState& state, const Weapon* weapon)
	{
		std::cout << "Do you want to equip it now? ( " << Keybinds::agree << '/' << Keybinds::disagree << " )\n";
		while (true)
		{
			char input{ getInput() };
			if (input == Keybinds::agree)
			{
				state.player.equipWeaponAtIndex(static_cast<int>(state.player.getWeapons().size()) - 1); // equip the last weapon in the inventory which is the one we just added ( AI suggested this )
				std::cout << "You equipped the " << weapon->getName() << '\n';
				break;
			}
			else if (input == Keybinds::disagree)
			{
				std::cout << "You can equip it whenever you want from your inventory\n";
				break;
			}
		}
	}

	void takeLoot(const FightState& state, const Item* loot)
	{
		state.player.addItem(*loot); // ( AI, just keeping it in for fun ) player is guaranteed to outlive the loot since the loot is only used in this function and the player is used in the fight which calls this function and the fight can't end without the player dying or the enemy dying and if the player dies then we won't get to this function and if the enemy dies then we will get to this function but the player will still be alive
									 // ( AI again ) also move semantics would be more appropriate here but it would require some changes to the inventory system and I don't want to do that rn
									 // can call non const function addItem on const state's member player because player is a reference
		std::cout << "The " << loot->getName() << " has been added to your inventory" << '\n';

		if (const Weapon* weapon{ dynamic_cast<const Weapon*>(loot) })
			equipWeaponLoot(state, weapon);
	}

	void leaveLoot(const Item* loot)
	{
		std::cout << "You left the " << loot->getName() << " with the corpse. Maybe out of respect?\n";
	}

	void lootEnemy(const FightState& state)
	{
		for (int i{}; i < state.enemy.getTier(); ++i)
		{
			waitForInput();

			const Item* loot = Loot::getRandomLoot(state.enemy.getTier());
			assert(loot && "Loot is null");
			std::cout << Aesthetics::clear << "The enemy had a " << loot->getName() << ", do you want to take it? ( " << Keybinds::agree << '/' << Keybinds::disagree << " )\n";

			while (true)
			{
				char c{ getInput() };

				if (c == Keybinds::agree)
				{
					takeLoot(state, loot);
					break;
				}
				else if (c == Keybinds::disagree)
				{
					leaveLoot(loot);
					break;
				}
			}
		}
	}

	void conclude(const FightState& state)
	{
		if (state.player.isDead())
			std::cout << "You died, quite embarrassingly...\n";
		else if (state.enemy.isDead())
		{
			std::cout << "You killed them, they are now just a soulless husk\n";
			lootEnemy(state);
			state.player.addXp(state.enemy.getXp());
			waitForInput();
		}
	}

	void fight(Player& player, Enemy& enemy)
	{
		FightState state{ player, enemy, [&]() { std::cout << "A fight has started between you and the " << enemy.getName() << '\n'; }, false, true };
		int indicator{};

		while (!state.finished && !player.isDead() && !enemy.isDead())
		{
			displayState(state);

			if (state.playersTurn)
				playersTurn(state, indicator);
			else
				enemysTurn(state);
		}

		displayState(state);
		conclude(state);
	}
}

int main()
{
	// maps
	Map map{ 9, 9 };
	Map map2{ 9, 9 };
	Map map3{ 9, 9 };
	Map map4{ 9, 9 };
	Map map5{ 9, 9 };

	// map one
	std::vector mapOneEnemies{
		Enemy{ &map, Point{ 7, 7 }, Enemy::goblin },
		Enemy{ &map, Point{ 1, 4 }, Enemy::goblin },
		Enemy{ &map, Point{ 7, 4 }, Enemy::goblin },
		Enemy{ &map, Point{ 1, 7 }, Enemy::goblin },
		Enemy{ &map, Point{ 4, 1 }, Enemy::exile },
	};
	
	std::vector mapOneWalls{
		Wall{ &map, Point{ 3, 0 } },
		Wall{ &map, Point{ 5, 0 } },
	};

	// map two
	std::vector mapTwoEnemies{
		Enemy{ &map2, Point{ 1, 6 }, Enemy::exile },
		Enemy{ &map2, Point{ 7, 6 }, Enemy::exile },
		Enemy{ &map2, Point{ 4, 2 }, Enemy::exile },
		Enemy{ &map2, Point{ 4, 6 }, Enemy::goblin },
		//Enemy{ &map2, Point{ 4, 2 }, Enemy::goblin },
		Enemy{ &map2, Point{ 4, 4 }, Enemy::knight },
		//Enemy{ &map2, Point{ 1, 2 }, Enemy::knight },
		//Enemy{ &map2, Point{ 7, 2 }, Enemy::knight },
	};

	std::vector mapTwoBlocks{
		Block{ Wall{ &map2, Point{} }, Point{ 0, 4 }, Point{ 3, 4 } },
		Block{ Wall{ &map2, Point{} }, Point{ 5, 4 }, Point{ 8, 4 } },
	};

	// map three
	std::vector mapThreeEnemies{
		Enemy{ &map3, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map3, Point{ 4, 1 }, Enemy::ogre },
	};

	std::vector mapThreeBlocks{
		Block{ Wall{ &map3, Point{} }, Point{ 0, 4 }, Point{ 3, 4 } },
		Block{ Wall{ &map3, Point{} }, Point{ 5, 4 }, Point{ 8, 4 } },
	};

	// map four
	std::vector mapFourEnemies{
		Enemy{ &map4, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map4, Point{ 1, 4 }, Enemy::ogre },
	};

	std::vector mapFourBlocks{
		Block{ Wall{ &map4, Point{} }, Point{ 4, 0 }, Point{ 4, 3 } },
		Block{ Wall{ &map4, Point{} }, Point{ 4, 5 }, Point{ 4, 8 } },
	};

	// map five
	std::vector mapFiveEnemies{
		Enemy{ &map5, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map5, Point{ 7, 4 }, Enemy::ogre },
	};

	std::vector mapFiveBlocks{
		Block{ Wall{ &map5, Point{} }, Point{ 4, 0 }, Point{ 4, 3 } },
		Block{ Wall{ &map5, Point{} }, Point{ 4, 5 }, Point{ 4, 8 } },
	};

	// door pairs
	std::vector doorPairs{
		DoorPair{ &map, Point{ 4, 0 }, &map2, Point{ 4, 8 } },
		DoorPair{ &map2, Point{ 4, 0 }, &map3, Point{ 4, 8 } },
		DoorPair{ &map2, Point{ 0, 2 }, &map4, Point{ 8, 4 } },
		DoorPair{ &map2, Point{ 8, 2 }, &map5, Point{ 0, 4 } },
	};

	Enemy dummy{ &map, Point{ 4, 6 }, Enemy::goblin };
	dummy.setDamage(0);
	dummy.setHealth(100);

	Player player{ &map, Point{ 4, 4 } };
	player.addItem(Weapon{ Weapon::stick });
	player.addItem(Potion{ Potion::healing });
	player.addItem(Food{ Food::bread });
	player.equipWeaponAtIndex(0);

	//player.addItem(Food{ Food::bread });
	//player.addItem(Food{ Food::bread });
	//player.addItem(Food{ Food::bread });
	//player.addItem(Food{ Food::bread });
	//player.addItem(Food{ Food::bread });
	//player.addItem(Food{ Food::bread_two });
	//player.addItem(Food{ Food::bread_two });
	//player.addItem(Food{ Food::bread_two });
	//player.addItem(Food{ Food::bread_two });
	//player.addItem(Food{ Food::bread_two });

	std::cout << std::left;

	while (!player.isDead())
	{
		std::cout << Aesthetics::clear << '\n';
		std::cout << "You are level " << player.getLevel() << "\n\n";
		std::cout << *(player.getMap()) << '\n';

		std::cout << "( " << Keybinds::movementKeybinds << " to move, " << Keybinds::inventory << " to open the inventory )\n";
		char c{ getInput() };

		Point dir{};
		if (c == Keybinds::up) dir = Directions::up;
		else if (c == Keybinds::down) dir = Directions::down;
		else if (c == Keybinds::left) dir = Directions::left;
		else if (c == Keybinds::right) dir = Directions::right;
		else if (c == Keybinds::inventory) Inventory::inventory(player);

		if (dir == Point{})
			continue;

		Entity* entity{ player.getMap()->getEntity(player.getPosition() + dir)};
		if (!entity)
			player.move(dir);
		else if (Enemy* opponent{ dynamic_cast<Enemy*>(entity) })
		{
			Fighting::fight(player, *opponent);

			if (opponent->isDead())
				opponent->deactivate();
		}
		else if (Door* door{ dynamic_cast<Door*>(entity) })
		{
			const Door* counterpart{};
			if (door == &(door->getDoorPair()->getFirst()))
				counterpart = &(door->getDoorPair()->getSecond());
			else
				counterpart = &(door->getDoorPair()->getFirst());

			player.setMap(counterpart->getMap());
			player.moveTo(counterpart->getPosition() + (door->getPosition() - player.getPosition()));
		}
	}

	std::cout << "You lost\n";

	getInput();

	return 0;
}
