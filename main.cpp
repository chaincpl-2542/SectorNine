#include <iostream>
#include <memory>
#include "GameEngine.hpp"
#include "Map.hpp"

using namespace CPL;

int main() 
{
	std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>(); //fix memory leak
	gameEngine->init();

	while (true) 
	{
		char input = gameEngine->handleInput();

		if (input == 'x')
			break;

		gameEngine->update(input);
		// Draw map
		gameEngine->render();
	}

	gameEngine->release();

	return 0;
}