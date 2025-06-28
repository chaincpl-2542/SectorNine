#pragma once

namespace CPL
{

    struct Room
    {
        int x;
        int y;
        int width;  
        int height; 

        int centerX() const { return x + width / 2; }
        int centerY() const { return y + height / 2; }

        bool intersects(const Room& other) const
        {
            return !(x + width <= other.x ||
                x >= other.x + other.width ||
                y + height <= other.y ||
                y >= other.y + other.height);
        }

        bool contains(int px, int py) const {
            return px >= x && px < x + width &&
                py >= y && py < y + height;
        }
    };

}
