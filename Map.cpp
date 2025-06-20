#include "Map.hpp"
#include "Entity.hpp"
#include <iostream>

namespace CPL 
{
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
}
