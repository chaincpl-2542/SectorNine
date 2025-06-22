#pragma once
#include "Entity.hpp"
#include "Map.hpp"
#include <memory>

namespace CPL 
{
    struct Node {
        int x, y;
        int gCost = 0;
        int hCost = 0;
        int fCost() const { return gCost + hCost; }

        Node* parent = nullptr;

        bool operator==(const Node& other) const {
            return x == other.x && y == other.y;
        }
    };

	class Enemy : public Entity
	{

	public:
		Enemy();
        std::vector<std::pair<int, int>> findPathToPlayer(
            const Map& map,
            int targetX,
            int targetY,
            const std::vector<std::shared_ptr<Enemy>>& allEnemies);
	};
}

