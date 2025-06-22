#include "Enemy.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <memory>
#include "Map.hpp"

namespace CPL
{
    Enemy::Enemy() :Entity(0, 0, 'E')
    {

    }
    struct NodeHash {
        std::size_t operator()(const Node& n) const {
            return std::hash<int>()(n.x) ^ (std::hash<int>()(n.y) << 1);
        }
    };

    std::vector<std::pair<int, int>> Enemy::findPathToPlayer(
        const Map& map,
        int targetX,
        int targetY,
        const std::vector<std::shared_ptr<Enemy>>& allEnemies)
    {
        int startX = getX();
        int startY = getY();

        std::vector<std::pair<int, int>> path;
        std::vector<std::vector<Node>> nodes(map.getHeight(), std::vector<Node>(map.getWidth()));

        for (int y = 0; y < map.getHeight(); ++y) {
            for (int x = 0; x < map.getWidth(); ++x) {
                nodes[y][x].x = x;
                nodes[y][x].y = y;
                nodes[y][x].gCost = INT_MAX;
                nodes[y][x].hCost = std::abs(x - targetX) + std::abs(y - targetY);
                nodes[y][x].parent = nullptr;
            }
        }

        auto compare = [](Node* a, Node* b) {
            return a->fCost() > b->fCost();
            };

        std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openSet(compare);

        Node* startNode = &nodes[startY][startX];
        startNode->gCost = 0;
        openSet.push(startNode);

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            if (current->x == targetX && current->y == targetY) {
                while (current->parent != nullptr) {
                    path.emplace_back(current->x, current->y);
                    current = current->parent;
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            const int dx[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
            const int dy[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

            for (int i = 0; i < 8; ++i) {
                int nx = current->x + dx[i];
                int ny = current->y + dy[i];

                if (!map.isWalkable(nx, ny))
                    continue;

                bool blockedByOther = false;
                for (const auto& e : allEnemies) {
                    if (e.get() == this) continue;
                    if (e->getX() == nx && e->getY() == ny) {
                        blockedByOther = true;
                        break;
                    }
                }
                if (blockedByOther) continue;

                Node* neighbor = &nodes[ny][nx];
                int moveCost = (i < 4) ? 10 : 14;
                int newG = current->gCost + moveCost;

                if (newG < neighbor->gCost) {
                    neighbor->gCost = newG;
                    neighbor->parent = current;
                    openSet.push(neighbor);
                }
            }
        }

        return path; // No path found
    }

}