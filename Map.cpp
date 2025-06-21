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

	void Map::Draw() 
	{
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++) 
			{
				if (x == 0 || x == width - 1 || y == 0 || y == height - 1) 
				{
					tiles[y][x] = '#';
				}
				else if (tiles[y][x] != '@' && tiles[y][x] != 'E')
				{
					tiles[y][x] = '.';
				}

				if (tiles[y][x] == '@')
					std::cout << "\033[1;32m" << '@' << "\033[0m"; // green
				else if (tiles[y][x] == 'E')
					std::cout << "\033[1;31m" << 'E' << "\033[0m"; // red
				else
					std::cout << tiles[y][x];
			}

			std::cout << std::endl;
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

	void Map::generateRooms(int numRooms)
	{
		std::cout << "Generating rooms..." << std::endl;

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		std::vector<Room> rooms;

		for (int i = 0; i < numRooms; ++i) {
			bool placed = false;
			for (int attempt = 0; attempt < 50 && !placed; ++attempt) {
				int w = 6 + std::rand() % 6;
				int h = 4 + std::rand() % 4;
				int x = 1 + std::rand() % (width - w - 2);
				int y = 1 + std::rand() % (height - h - 2);

				Room newRoom = { x, y, w, h };

				bool overlap = false;
				for (const Room& other : rooms) {
					if (newRoom.intersects(other)) {
						overlap = true;
						break;
					}
				}

				if (!overlap) {
					// draw room in map
					for (int iy = y; iy < y + h; ++iy)
						for (int ix = x; ix < x + w; ++ix)
							tiles[iy][ix] = '.';

					rooms.push_back(newRoom);

					// place player in first room
					if (rooms.size() == 1) {
						tiles[y + h / 2][x + w / 2] = '@';
					}

					placed = true;
				}
			}
		}
	}
}
