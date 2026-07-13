#include <iomanip>
#include <iostream>
#include <vector>
// -
#include "Aesthetics.h"
#include "Keybinds.h"
#include "Point.h"
#include "Map.h"
#include "Entity.h"
#include "CreatureBase.h"
#include "Weapon.h"
#include "Potion.h"
#include "Food.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Door.h"
#include "DoorPair.h"
#include "Block.h"
#include "Input.h"
#include "Inventory.h"
#include "Fighting.h"

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

	std::vector mapOneBlocks{
		Block{ Wall{ &map, Point{} }, Point{ 3, 0 }, Point{ 5, 0 }, Point{ 1, 0 } },
	};

	// map two
	std::vector mapTwoEnemies{
		Enemy{ &map2, Point{ 1, 6 }, Enemy::exile },
		Enemy{ &map2, Point{ 7, 6 }, Enemy::exile },
		Enemy{ &map2, Point{ 4, 2 }, Enemy::exile },
		Enemy{ &map2, Point{ 4, 6 }, Enemy::goblin },
		Enemy{ &map2, Point{ 4, 4 }, Enemy::knight },
	};

	std::vector mapTwoBlocks{
		Block{ Wall{ &map2, Point{} }, Point{ 0, 4 }, Point{ 3, 4 } },
		Block{ Wall{ &map2, Point{} }, Point{ 5, 4 }, Point{ 8, 4 } },
		Block{ Wall{ &map2, Point{} }, Point{ 3, 8 }, Point{ 5, 8 }, Point{ 1, 0 } },
		Block{ Wall{ &map2, Point{} }, Point{ 3, 0 }, Point{ 5, 0 }, Point{ 1, 0 } },
		Block{ Wall{ &map2, Point{} }, Point{ 0, 1 }, Point{ 0, 3 }, Point{ 0, 1 } },
		Block{ Wall{ &map2, Point{} }, Point{ 8, 1 }, Point{ 8, 3 }, Point{ 0, 1 } },
	};

	// map three
	std::vector mapThreeEnemies{
		Enemy{ &map3, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map3, Point{ 4, 1 }, Enemy::ogre },
	};

	std::vector mapThreeBlocks{
		Block{ Wall{ &map3, Point{} }, Point{ 0, 4 }, Point{ 3, 4 } },
		Block{ Wall{ &map3, Point{} }, Point{ 5, 4 }, Point{ 8, 4 } },
		Block{ Wall{ &map3, Point{} }, Point{ 3, 8 }, Point{ 5, 8 }, Point{ 1, 0 } },
	};

	// map four
	std::vector mapFourEnemies{
		Enemy{ &map4, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map4, Point{ 1, 4 }, Enemy::ogre },
	};

	std::vector mapFourBlocks{
		Block{ Wall{ &map4, Point{} }, Point{ 4, 0 }, Point{ 4, 3 } },
		Block{ Wall{ &map4, Point{} }, Point{ 4, 5 }, Point{ 4, 8 } },
		Block{ Wall{ &map4, Point{} }, Point{ 8, 3 }, Point{ 8, 5 }, Point{ 0, 1 } },
	};

	// map five
	std::vector mapFiveEnemies{
		Enemy{ &map5, Point{ 4, 4 }, Enemy::knight },
		Enemy{ &map5, Point{ 7, 4 }, Enemy::ogre },
	};

	std::vector mapFiveBlocks{
		Block{ Wall{ &map5, Point{} }, Point{ 4, 0 }, Point{ 4, 3 } },
		Block{ Wall{ &map5, Point{} }, Point{ 4, 5 }, Point{ 4, 8 } },
		Block{ Wall{ &map5, Point{} }, Point{ 0, 3 }, Point{ 0, 5 }, Point{ 0, 1 } },
	};

	// door pairs
	std::vector doorPairs{
		DoorPair{ &map, Point{ 4, 0 }, &map2, Point{ 4, 8 } },
		DoorPair{ &map2, Point{ 4, 0 }, &map3, Point{ 4, 8 } },
		DoorPair{ &map2, Point{ 0, 2 }, &map4, Point{ 8, 4 } },
		DoorPair{ &map2, Point{ 8, 2 }, &map5, Point{ 0, 4 } },
	};

	Player player{ &map, Point{ 4, 4 } };
	player.addItem(Weapon{ Weapon::stick });
	player.addItem(Potion{ Potion::healing });
	player.addItem(Food{ Food::bread });
	player.equipWeaponAtIndex(0);

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
