#pragma once
#include <vector>
#include <memory>
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

		//Door
		void toggleDoor(int x, int y);
		bool isDoorOpen(int x, int y) const;
		bool isDoor(int x, int y) const;

		std::pair<int, int> getPlayerStart() const;
	private:

		std::pair<int, int> playerStart{ 1,1 };

		void carveHorizontal(int x1, int x2, int y);
		void carveVertical(int y1, int y2, int x);
		void connectLeafs(std::shared_ptr<Leaf> node);

		void digPathWithDoor(int x1, int y1, int x2, int y2);

		const unsigned int width = 100;
		const unsigned int height = 35;
		int doorChance = 40;

		std::vector<std::vector<char>>tiles;
		std::vector<std::vector<bool>>doorOpen;
	};
}

