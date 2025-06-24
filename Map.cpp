#include "Map.hpp"
#include "Entity.hpp"
#include "Leaf.hpp"
#include "Room.hpp"
#include <iostream>

namespace CPL 
{
	Map::Map() 
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
				char c = tiles[y][x];

				if (c == '+') {
					bool open = doorOpen[y][x];
					std::cout << (open ? "\033[1;32m+\033[0m"
						: "\033[1;31m+\033[0m");
					continue;
				}

				if (c == '@') {
					std::cout << "\033[1;32m@\033[0m";
					continue;
				}
				else if (c == 'E') {
					std::cout << "\033[1;31mE\033[0m";
					continue;
				}
				else
					std::cout << c;
			}
			std::cout << '\n';
		}
	}


	void Map::DrawEntities(const Entity& entity)
	{
		int x = entity.getX();
		int y = entity.getY();
		char symbol = entity.getSymbol();
		if (tiles[y][x] != '+')
			tiles[y][x] = symbol;

		if(symbol == '@')
			std::cout << "Player: (" << x << ", " << y << ")\n";
	}

	void Map::ClearEntities()
	{
		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				if (tiles[y][x] == '@' || tiles[y][x] == 'E')
					tiles[y][x] = '.';
			}
		}
	}

	bool Map::isWalkable(int x, int y) const
	{
		if (y < 0 || y >= height || x < 0 || x >= width) return false;

		char t = tiles[y][x];
		if (t == '+')
			return doorOpen[y][x];
		else
			return t == '.' || t == '@';
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
		default: return WALL;
		}
	}

	void Map::toggleDoor(int x, int y)
	{
		if (tiles[y][x] == '+')
			doorOpen[y][x] = !doorOpen[y][x];
	}

	bool Map::isDoorOpen(int x, int y) const
	{
		return tiles[y][x] == '+' && doorOpen[y][x];
	}

	void Map::generateRoomsBSP()
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		auto root = std::make_shared<Leaf>(1, 1, width - 2, height - 2);

		std::vector<std::shared_ptr<Leaf>> leafs;
		leafs.push_back(root);
		bool didSplit = true;
		while (didSplit)
		{
			didSplit = false;
			std::vector<std::shared_ptr<Leaf>> newLeafs;

			for (auto& l : leafs)
			{
				if (!l->leftChild && !l->rightChild)
				{
					if ((l->width > Leaf::MIN_LEAF_SIZE * 2) ||
						(l->height > Leaf::MIN_LEAF_SIZE * 2))
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
		for (auto& l : leafs)
		{
			if (!l->room) continue;
			Room r = l->room.value();

			for (int y = r.y; y < r.y + r.height; ++y)
				for (int x = r.x; x < r.x + r.width; ++x)
					tiles[y][x] = '.';

			if (!firstRoomPlaced)
			{
				int cx = r.centerX();
				int cy = r.centerY();
				playerStart = { cx, cy };
				firstRoomPlaced = true;
			}
		}


		connectLeafs(root);

		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				if (tiles[y][x] != '#') continue;

				bool nearFloor =
					tiles[y - 1][x] == '.' || tiles[y + 1][x] == '.'
					|| tiles[y][x - 1] == '.' || tiles[y][x + 1] == '.';

				if (!nearFloor)
					tiles[y][x] = ' ';
			}
		}
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

	bool Map::isDoor(int x, int y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height) return false;
		return tiles[y][x] == '+';
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
