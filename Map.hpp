#pragma once
#include <vector>
#include "Entity.hpp"
#include "Leaf.hpp"

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

		void Draw() const;

		void DrawEntities(const Entity& entity);
		void ClearEntities();
		bool isWalkable(int x, int y) const;
		TileType getTileType(int x, int y) const;

		//Get map size
		void generateRoomsBSP();

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		std::pair<int, int> getPlayerStart() const;
	private:
		std::pair<int, int> playerStart{ 1,1 };

		void carveHorizontal(int x1, int x2, int y);
		void carveVertical(int y1, int y2, int x);
		void connectLeafs(std::shared_ptr<Leaf> node);

		const unsigned int width = 80;
		const unsigned int height = 25;
		 std::vector<std::vector<char>>tiles;
	};
}

