#pragma once
#include <vector>
#include "Entity.hpp"

namespace CPL
{
	enum TileType {
		FLOOR,
		WALL,
		ENEMY
	};
	class Map
	{
	public:
		Map();
		void Draw();

		void DrawEntities(const Entity& entity);
		void ClearEntities();
		bool isWalkable(int x, int y) const;
		TileType getTileType(int x, int y) const;
		void generateRooms(int numRooms);

		//Get map size
		unsigned int getWidth() const;
		unsigned int getHeight() const;
	private:
		const unsigned int width = 80;
		const unsigned int height = 25;
		 std::vector<std::vector<char>>tiles;
	};
}

