#include "Map.hpp"
#include "Entity.hpp"
#include <iostream>

namespace CPL 
{
	struct Room {
		int x, y;
		int width, height;

		bool intersects(const Room& other) const {
			return !(x + width <= other.x || x >= other.x + other.width ||
				y + height <= other.y || y >= other.y + other.height);
		}
	};

	Map::Map() 
	{
		tiles.resize(height, std::vector<char>(width, '.'));
	}

	void Map::Draw() const
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				char c = tiles[y][x];

				if (c == '@')
					std::cout << "\033[1;32m" << c << "\033[0m";
				else if (c == 'E')
					std::cout << "\033[1;31m" << c << "\033[0m";
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
		if (y < 0 || y >= height || x < 0 || x >= width)
			return false;

		char tile = tiles[y][x];
		return tile != '#';
	}

	TileType Map::getTileType(int x, int y) const {
		if (y < 0 || y >= height || x < 0 || x >= width)
			return WALL;

		char tile = tiles[y][x];
		switch (tile) {
		case '#': return WALL;
		case 'E': return ENEMY;
		default:  return FLOOR;
		}
	}


	void Map::generateRooms(int minLeafSize)
	{
		// 1) ��駤�Ҿ�鹷��������繡�ᾧ��͹
		tiles.assign(height, std::vector<char>(width, '#'));

		// 2) ����� root leaf
		Leaf root{ 1, 1, static_cast<int>(width - 2), static_cast<int>(height - 2) };
		std::vector<Leaf*> leaves{ &root };

		// 3) �觷������ͧ���������
		bool didSplit = true;
		while (didSplit)
		{
			didSplit = false;
			std::vector<Leaf*> next;
			for (Leaf* l : leaves)
			{
				if (!l->left && !l->right && l->split(minLeafSize))
				{
					// ���������� �����١ŧ list
					next.push_back(l->left);
					next.push_back(l->right);
					didSplit = true;
				}
				else
					next.push_back(l);
			}
			leaves.swap(next);
		}

		// 4) ���ҧ��ͧ����� leaf
		std::vector<Room> rooms;
		root.createRooms(*this, rooms);

		// 5) �ҧ�����蹵ç��ҧ��ͧ�á (�����)
		if (!rooms.empty())
		{
			const Room& r = rooms.front();
			int px = r.x + r.w / 2;
			int py = r.y + r.h / 2;
			tiles[py][px] = '@';     // Player start
		}
	}

	bool Map::Leaf::split(int minLeaf)
	{
		if (left || right) return false;            // �١�������

		bool splitH = (rand() % 2) == 0;
		if (w > h && static_cast<float>(w) / h >= 1.25f) splitH = false;
		else if (h > w && static_cast<float>(h) / w >= 1.25f) splitH = true;

		int max = (splitH ? h : w) - minLeaf;
		if (max <= minLeaf) return false;           // ����Թ��

		int splitPos = minLeaf + rand() % (max - minLeaf);

		if (splitH)
		{   // ���ǹ͹
			left = new Leaf{ x,     y,     w, splitPos };
			right = new Leaf{ x, y + splitPos, w, h - splitPos };
		}
		else
		{   // ���ǵ��
			left = new Leaf{ x, y, splitPos, h };
			right = new Leaf{ x + splitPos, y, w - splitPos, h };
		}
		return true;
	}

	void Map::Leaf::createRooms(Map& map, std::vector<Room>& rooms)
	{
		if (left || right)
		{
			// ���١ => �����
			if (left)  left->createRooms(map, rooms);
			if (right) right->createRooms(map, rooms);

			// �������ͧ�١����-���
			if (left && right)
			{
				Room a = left->room;
				Room b = right->room;
				int ax = a.x + a.w / 2;
				int ay = a.y + a.h / 2;
				int bx = b.x + b.w / 2;
				int by = b.y + b.h / 2;

				// �Թ�ǹ͹��͹����ŧ/���
				for (int x = std::min(ax, bx); x <= std::max(ax, bx); ++x) map.tiles[ay][x] = '.';
				for (int y = std::min(ay, by); y <= std::max(ay, by); ++y) map.tiles[y][bx] = '.';
			}
		}
		else
		{
			// ��ش���� => �ش��ͧ
			int roomW = 4 + rand() % (w - 4);
			int roomH = 4 + rand() % (h - 4);
			int roomX = x + rand() % (w - roomW);
			int roomY = y + rand() % (h - roomH);

			room = { roomX, roomY, roomW, roomH };

			for (int iy = roomY; iy < roomY + roomH; ++iy)
				for (int ix = roomX; ix < roomX + roomW; ++ix)
					map.tiles[iy][ix] = '.';

			rooms.push_back(room);
		}
	}

	std::pair<int, int> Map::getPlayerStart() const
	{
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				if (tiles[y][x] == '@')
					return { x, y };
		return { 1, 1 };
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
