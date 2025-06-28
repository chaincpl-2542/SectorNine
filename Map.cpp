#include "Map.hpp"
#include "Entity.hpp"
#include "Leaf.hpp"
#include "Room.hpp"
#include <iostream>

namespace CPL 
{
	Map::Map(int w, int h, int chance) : width(w), height(h), doorChance(chance)
	{
		tiles.resize(height, std::vector<char>(width, '#'));

		doorOpen.resize(height, std::vector<bool>(width, false));
	}

	void Map::Draw() const
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (!revealed[y][x]) {
					std::cout << ' ';
					continue;
				}

				if (!visible[y][x])
				{
					char c = tiles[y][x];

					if (c == '#' || c == '+' || c == 'L' || c == 'O')
					{
						std::cout << c;
					}
					else
					{
						std::cout << ' ';
					}
					continue;
				}

				char c = tiles[y][x];

				if (c == '+') {
					std::cout << (doorOpen[y][x] ? 
						"\033[1;32m+\033[0m" : 
						"\033[1;31m+\033[0m");
				}
				else if (c == 'L')
				{
					std::cout << (doorOpen[y][x] ?
						"\033[1;32mL\033[0m" :
						"\033[1;31mL\033[0m");
				}
				else if (c == 'P') {
					std::cout << "\033[1;33mP\033[0m";
				}
				else if (c == 'O') {
					std::cout << "\033[1;33mO\033[0m";
				}
				else if (c == '@') {
					std::cout << "\033[1;32m@\033[0m";
				}
				else if (c == 'E') {
					std::cout << "\033[1;31mE\033[0m";
				}
				else if (c == 'M') {
					std::cout << "\033[1;34mM\033[0m";
				}
				else {
					std::cout << c;
				}
			}
			std::cout << '\n';
		}
	}



	void Map::DrawEntities(const Entity& entity)
	{
		int x = entity.getX();
		int y = entity.getY();
		char symbol = entity.getSymbol();
		if (tiles[y][x] == '+' || (tiles[y][x] == 'L' && !doorOpen[y][x]))
			return;
		tiles[y][x] = symbol;
	}

	void Map::ClearEntities()
	{
		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				if (tiles[y][x] == '@' || tiles[y][x] == 'E')
				{
					if (doorOpen[y][x])
						tiles[y][x] = 'L';
					else
						tiles[y][x] = '.';
				}
			}
		}
	}

	bool Map::isWalkable(int x, int y) const
	{
		if (y < 0 || y >= height || x < 0 || x >= width) return false;

		char t = tiles[y][x];

		if (t == '+') 
			return doorOpen[y][x];
		if (t == 'L') 
			return doorOpen[y][x]; 
		if (t == 'M') 
			return true;
		return t == '.' || t == '@' || t == 'P' || t == 'O';
	}


	TileType Map::getTileType(int x, int y) const
	{
		if (y < 0 || y >= height || x < 0 || x >= width) return WALL;

		char t = tiles[y][x];
		switch (t)
		{
		case '+': return doorOpen[y][x] ? FLOOR : WALL;
		case '.': return FLOOR;
		case 'E': return ENEMY;
		case 'O': return ESCAPE;
		case 'P': return PASS_KEY;
		case 'L': return doorOpen[y][x] ? FLOOR : LOCKED_DOOR;
		case 'M': return MAP;
		default:  return WALL;
		}
	}

	void Map::toggleDoor(int x, int y)
	{
		if (tiles[y][x] == '+' || tiles[y][x] == 'L')
			doorOpen[y][x] = !doorOpen[y][x];
	}

	bool Map::isDoorOpen(int x, int y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height) return false;
		return (tiles[y][x] == '+' || tiles[y][x] == 'L') && doorOpen[y][x];
	}

	void Map::setDoorOpen(int x, int y, bool open)
	{
		if (tiles[y][x] == '+' || tiles[y][x] == 'L')
			doorOpen[y][x] = open;
	}

	void Map::unlockAllLockedDoors()
	{
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				if (tiles[y][x] == 'L')
					doorOpen[y][x] = true;
	}

	void Map::generateRoomsBSP()
	{
		visible.assign(height, std::vector<bool>(width, false));
		revealed.assign(height, std::vector<bool>(width, false));

		srand(static_cast<unsigned int>(time(nullptr)));

		auto root = std::make_shared<Leaf>(1, 1, width - 2, height - 2);

		std::vector<std::shared_ptr<Leaf>> leafs{ root };
		bool didSplit = true;

		while (didSplit)
		{
			didSplit = false;
			std::vector<std::shared_ptr<Leaf>> newLeafs;

			for (auto& l : leafs)
			{
				if (!l->leftChild && !l->rightChild)
				{
					if (l->width > Leaf::MIN_LEAF_SIZE * 2 ||
						l->height > Leaf::MIN_LEAF_SIZE * 2)
					{
						if (l->split())
						{
							newLeafs.push_back(l->leftChild);
							newLeafs.push_back(l->rightChild);
							didSplit = true;
						}
					}
				}
			}
			leafs.insert(leafs.end(), newLeafs.begin(), newLeafs.end());
		}

		root->createRooms();

		bool firstRoomPlaced = false;
		int  maxDist = -1;
		std::pair<int, int> escapePos{ 1,1 };

		for (auto& l : leafs)
		{
			if (!l->room) continue;
			Room r = l->room.value();

			for (int y = r.y; y < r.y + r.height; ++y)
				for (int x = r.x; x < r.x + r.width; ++x)
					tiles[y][x] = '.';

			if (!firstRoomPlaced)
			{
				playerStart = { r.centerX(), r.centerY() };
				firstRoomPlaced = true;
			}

			int dist = std::abs(r.centerX() - playerStart.first) +
				std::abs(r.centerY() - playerStart.second);

			if (dist > maxDist)
			{
				maxDist = dist;
				escapePos = { r.centerX(), r.centerY() };
			}
		}

		connectLeafs(root);

		setTile(escapePos.first, escapePos.second, 'O');

		Room exitRoom;
		for (auto& l : leafs) {
			if (l->room && l->room->contains(escapePos.first, escapePos.second)) {
				exitRoom = l->room.value();
				break;
			}
		}

		auto lockTile = [&](int x, int y)
			{
				if (tiles[y][x] == '+' || tiles[y][x] == '.')
				{
					tiles[y][x] = 'L';
					doorOpen[y][x] = false;
				}
			};

		for (int x = exitRoom.x; x < exitRoom.x + exitRoom.width; ++x)
		{
			lockTile(x, exitRoom.y - 1);
			lockTile(x, exitRoom.y + exitRoom.height);
		}
		for (int y = exitRoom.y; y < exitRoom.y + exitRoom.height; ++y)
		{
			lockTile(exitRoom.x - 1, y);
			lockTile(exitRoom.x + exitRoom.width, y);
		}

		bool keyPlaced = false;
		while (!keyPlaced)
		{
			auto& leaf = leafs[rand() % leafs.size()];
			if (!leaf->room) continue;

			Room r = leaf->room.value();
			if (r.contains(escapePos.first, escapePos.second)) continue;

			int kx = r.centerX();
			int ky = r.centerY();
			if (tiles[ky][kx] == '.')
			{
				tiles[ky][kx] = 'P';
				keyPlaced = true;
			}
		}

		bool mapPlaced = false;
		while (!mapPlaced)
		{
			auto& leaf = leafs[rand() % leafs.size()];
			if (!leaf->room) continue;

			Room r = leaf->room.value();
			if (r.contains(escapePos.first, escapePos.second)) continue;

			int mx = r.centerX();
			int my = r.centerY();
			if (tiles[my][mx] == '.')
			{
				tiles[my][mx] = 'M';
				mapPlaced = true;
			}
		}

		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				if (tiles[y][x] != '#') continue;

				bool nearFloor = tiles[y - 1][x] == '.' || tiles[y + 1][x] == '.' ||
					tiles[y][x - 1] == '.' || tiles[y][x + 1] == '.';

				if (!nearFloor) tiles[y][x] = ' ';
			}
		}
	}


	void Map::setTile(int x, int y, char symbol)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
			tiles[y][x] = symbol;
	}

	char Map::getTile(int x, int y) const
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
			return tiles[y][x];
		return '#';
	}

	void Map::carveHorizontal(int x1, int x2, int y)
	{
		if (x1 > x2) std::swap(x1, x2);
		for (int x = x1; x <= x2; ++x)
			if (tiles[y][x] != '+') tiles[y][x] = '.';
	}

	void Map::carveVertical(int y1, int y2, int x)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y <= y2; ++y)
			if (tiles[y][x] != '+') tiles[y][x] = '.';
	}

	void Map::connectLeafs(std::shared_ptr<Leaf> node)
	{
		if (!node || !node->leftChild || !node->rightChild) return;

		Room a = node->leftChild->getRoom().value();
		Room b = node->rightChild->getRoom().value();

		int ax = a.centerX(), ay = a.centerY();
		int bx = b.centerX(), by = b.centerY();

		if (rand() % 2)
			digPathWithDoor(ax, ay, bx, ay), digPathWithDoor(bx, ay, bx, by);
		else
			digPathWithDoor(ax, ay, ax, by), digPathWithDoor(ax, by, bx, by);

		connectLeafs(node->leftChild);
		connectLeafs(node->rightChild);
	}

	void Map::digPathWithDoor(int x1, int y1, int x2, int y2)
	{
		int dx = (x2 > x1) ? 1 : -1;
		bool doorPlaced = false;

		for (int x = x1; x != x2 + dx; x += dx)
		{
			if (tiles[y1][x] == '#')
			{
				if (!doorPlaced)
				{
					if (rand() % 100 < doorChance)
					{
						tiles[y1][x] = '+';
						doorOpen[y1][x] = false;
					}
					else
					{
						tiles[y1][x] = '.';
					}
					doorPlaced = true;
				}
				else
					tiles[y1][x] = '.';
			}
			else if (tiles[y1][x] != '+')
				tiles[y1][x] = '.';
		}

		int dy = (y2 > y1) ? 1 : -1;
		doorPlaced = false;

		for (int y = y1 + dy; y != y2 + dy; y += dy)
		{
			if (tiles[y][x2] == '#')
			{
				if (!doorPlaced)
				{
					if (rand() % 100 < doorChance)
					{
						tiles[y][x2] = '+';
						doorOpen[y][x2] = false;
					}
					else
					{
						tiles[y][x2] = '.';
					}
					doorPlaced = true;
				}
				else
					tiles[y][x2] = '.';
			}
			else if (tiles[y][x2] != '+')
				tiles[y][x2] = '.';
		}
	}

	void Map::ShowStaticMapOnly() const
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				char c = tiles[y][x];

				if (c == '#')
				{
					std::cout << '#';
				}
				else if (c == 'P')
				{
					std::cout << "\033[1;33mP\033[0m";
				}
				else if (c == 'L')
				{
					std::cout << (doorOpen[y][x] ? 
						"\033[1;32mL\033[0m": 
						"\033[1;31mL\033[0m");
				}
				else if (c == '+')
				{
					std::cout << (doorOpen[y][x] ? "\033[1;32m+\033[0m" : "\033[1;31m+\033[0m");
				}
				else if (c == 'O')
				{
					std::cout << "\033[1;33mO\033[0m";
				}
				else if (c == 'M')
				{
					std::cout << "\033[1;34mM\033[0m";
				}
				else
				{
					std::cout << ' ';
				}
			}
			std::cout << '\n';
		}
	}

	void Map::revealWholeDungeon()
	{
		for (auto& row : revealed)
			std::fill(row.begin(), row.end(), true);
	}


	bool Map::isDoor(int x, int y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height) return false;
		return tiles[y][x] == '+' || tiles[y][x] == 'L';
	}

	std::pair<int, int> Map::getPlayerStart() const
	{
		return playerStart;
	}

	unsigned int Map::getWidth() const
	{
		return width;
	}
	unsigned int Map::getHeight() const
	{
		return height;
	}
}
