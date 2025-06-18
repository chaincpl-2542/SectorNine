#include "GameEngine.hpp"
#include <iostream>

namespace CPL 
{
	GameEngine::GameEngine()
	{

	}

	void GameEngine::init()
	{
		std::cout << "Init" << std::endl;
		map = std::make_unique<Map>();
		player = std::make_unique<Player>();

		enemies.push_back(std::make_shared<Enemy>());
		enemies.push_back(std::make_shared<Enemy>());
		enemies.push_back(std::make_shared<Enemy>());
	}

	int GameEngine::handleInput()
	{
		std::cout << "What do you want to do?" << std::endl;
		std::cout << "w  = up" << std::endl;
		std::cout << "s = down" << std::endl;
		std::cout << "a = left" << std::endl;
		std::cout << "d = right" << std::endl;
		std::cout << "x = exit" << std::endl;

		char input;
		std::cin >> input;
		if (input == 'x') 
			return 0;
		else
			return 1;
	}

	void GameEngine::update()
	{

	}

	void GameEngine::render()
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif

		std::cout << "Render" << std::endl;
		map->draw();
		std::cout << static_cast<char>(player->getSymbol());

		for (int i; i < enemies.size(); i++) {
			std::cout << static_cast<char>(enemies[i]->getSymbol());
		}
	}

	void GameEngine::release()
	{
		std::cout << "Release" << std::endl;

	}
}
