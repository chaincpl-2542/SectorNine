#pragma once
#include <memory>
#include <optional>
#include "Room.hpp"

namespace CPL {

    class Leaf {
    public:
        static const int MIN_LEAF_SIZE = 10;

        Leaf(int x, int y, int width, int height);

        bool split();

        void createRoom();

        void createRooms();

        std::shared_ptr<Leaf> leftChild;
        std::shared_ptr<Leaf> rightChild;
        std::optional<Room> room;

        int x, y, width, height;

        std::optional<Room> getRoom();
    };
}