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

		void generateRooms(int minLeafSize = 10);

		void Draw() const;

		void DrawEntities(const Entity& entity);
		void ClearEntities();
		bool isWalkable(int x, int y) const;
		TileType getTileType(int x, int y) const;

		//Get map size
		unsigned int getWidth() const;
		unsigned int getHeight() const;

		std::pair<int, int> getPlayerStart() const;

	private:
		struct Room { int x, y, w, h; };                   // �红�������ͧ��������
		struct Leaf                                          // �˹� BSP
		{
			int x, y, w, h;
			Leaf* left = nullptr;
			Leaf* right = nullptr;
			Room  room{ 0,0,0,0 };

			bool split(int minLeaf);                        // ���˹�
			void createRooms(Map& map, std::vector<Room>&); // �ش��ͧ + ⶧
		};

		const unsigned int width = 80;
		const unsigned int height = 25;
		 std::vector<std::vector<char>>tiles;
	};
}

