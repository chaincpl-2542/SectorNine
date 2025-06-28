#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "Leaf.hpp"

namespace CPL
{
	enum TileType {
		FLOOR, //'.'
		WALL, //'#'
		ENEMY, //'E'
		ESCAPE, //'O'
		DOOR, //'+'
		LOCKED_DOOR, //'L'
		PASS_KEY, //'P'
		MAP //'M'
	};
	class Map
	{
	public:
		Map(int w, int h, int chance);

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
		void setDoorOpen(int x, int y, bool open);
		bool isDoor(int x, int y) const;

		std::pair<int, int> getPlayerStart() const;
		void UpdateVisibility(int px, int py, int radius);

		void setTile(int x, int y, char symbol);
		char getTile(int x, int y) const;
		std::pair<int, int> getFarthestFloor(int fromX, int fromY) const;

		void ShowStaticMapOnly() const;

		void unlockAllLockedDoors();
		void revealWholeDungeon();

	private:

		std::pair<int, int> playerStart{ 1,1 };

		void carveHorizontal(int x1, int x2, int y);
		void carveVertical(int y1, int y2, int x);
		void connectLeafs(std::shared_ptr<Leaf> node);

		void digPathWithDoor(int x1, int y1, int x2, int y2);

		const unsigned int width = 80;
		const unsigned int height = 30;
		int doorChance = 60;

		std::vector<std::vector<char>> tiles;
		std::vector<std::vector<bool>> doorOpen;
		std::vector<std::vector<bool>> doorLocked;

		std::vector<std::vector<bool>> visible;
		std::vector<std::vector<bool>> revealed;
	};
}