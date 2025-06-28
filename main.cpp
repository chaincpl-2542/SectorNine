#include <iostream>
#include <memory>
#include <conio.h> 
#include "GameEngine.hpp"
#include "Map.hpp"

using namespace CPL;

int main()
{
    using namespace CPL;

    GameEngine engine;
    engine.init();

    bool waitingRestart = false;

    while (true)
    {
        engine.render();
        if (engine.isGameOver())
        {
            if (!waitingRestart)
            {
                waitingRestart = true;
                continue;
            }

            while (_kbhit()) _getch();
            _getch();
            engine.restart();

            waitingRestart = false;
            continue;
        }

        if (engine.gameWon)
        {
            std::cout << "\n*** YOU ESCAPED! ***\n";
            break;
        }

        char ch = engine.handleInput();
        if (ch == 'x') break;

        engine.update(ch);
    }
}