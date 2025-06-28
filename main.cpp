#include <iostream>
#include <memory>
#include "GameEngine.hpp"
#include "Map.hpp"

using namespace CPL;

int main() 
{
	std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>(); //fix memory leak
	gameEngine->init();

	while (!gameEngine->gameWon) 
	{
		char input = gameEngine->handleInput();

		if (input == 'x')
			break;

		gameEngine->update(input);
		// Draw map
		gameEngine->render();
	}
	std::cout << "\n*** YOU ESCAPED SECTOR NINE! ***\n";
	gameEngine->release();

	return 0;
}