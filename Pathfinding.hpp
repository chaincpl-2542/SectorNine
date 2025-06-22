#pragma once

#include <vector>
#include <utility>

namespace CPL {

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

}
