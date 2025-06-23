#include "Leaf.hpp"
#include <algorithm>  
#include <cstdlib>   

namespace CPL {

    Leaf::Leaf(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height),
        leftChild(nullptr), rightChild(nullptr) {}

    bool Leaf::split()
    {
        if (leftChild || rightChild) return false;

        bool splitH;
        if (width > height && static_cast<float>(width) / height >= 1.25f)
            splitH = false;                   
        else if (height > width && static_cast<float>(height) / width >= 1.25f)
            splitH = true;                       
        else
            splitH = rand() % 2;                  

        int max = (splitH ? height : width) - MIN_LEAF_SIZE;
        if (max <= MIN_LEAF_SIZE) return false;

        int splitPos = MIN_LEAF_SIZE + rand() % (max - MIN_LEAF_SIZE + 1);

        if (splitH)
        {
            leftChild = std::make_shared<Leaf>(x, y, width, splitPos);
            rightChild = std::make_shared<Leaf>(x, y + splitPos, width, height - splitPos);
        }
        else
        {
            leftChild = std::make_shared<Leaf>(x, y, splitPos, height);
            rightChild = std::make_shared<Leaf>(x + splitPos, y, width - splitPos, height);
        }
        return true;
    }

    void Leaf::createRoom()
    {
        int roomWidth = 4 + rand() % std::max(1, width - 4);
        int roomHeight = 4 + rand() % std::max(1, height - 4);

        int maxOffsetX = std::max(0, width - roomWidth - 2);
        int maxOffsetY = std::max(0, height - roomHeight - 2);

        int roomX = x + 1 + (maxOffsetX ? rand() % maxOffsetX : 0);
        int roomY = y + 1 + (maxOffsetY ? rand() % maxOffsetY : 0);

        room = Room{ roomX, roomY, roomWidth, roomHeight };
    }

    void Leaf::createRooms()
    {
        if (leftChild || rightChild)
        {
            if (leftChild)  leftChild->createRooms();
            if (rightChild) rightChild->createRooms();
        }
        else
        {
            createRoom();
        }
    }

    std::optional<Room> Leaf::getRoom()
    {
        if (room.has_value()) return room;

        std::optional<Room> lRoom;
        std::optional<Room> rRoom;

        if (leftChild)  lRoom = leftChild->getRoom();
        if (rightChild) rRoom = rightChild->getRoom();

        if (!lRoom && !rRoom) return std::nullopt;
        if (!rRoom)           return lRoom;
        if (!lRoom)           return rRoom;

        return (rand() % 2) ? lRoom : rRoom;
    }

} 
