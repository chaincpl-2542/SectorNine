#include "GameEngine.hpp"
#include "Player.hpp"
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
		player->setPosition(2, 2);

		enemies.push_back(std::make_shared<Enemy>());
		enemies.push_back(std::make_shared<Enemy>());
		enemies.push_back(std::make_shared<Enemy>());
		map->Draw();
	}

	char GameEngine::handleInput()
	{
		std::cout << "w  = up, s = down, a = left, d = right" << std::endl;
		std::cout << "x = exit" << std::endl;

		char input;
		std::cin >> input;

		return input;
	}

	void GameEngine::update(char input)
	{
		int x = player->getX();
		int y = player->getY();

		switch (input)
		{
		case 'w': y--;
			break;
		case 's': y++;
			break;
		case 'a': x--;
			break;
		case 'd': x++;
			break;
		default:
			break;
		}

		player->setPosition(x, y);
	}

	void GameEngine::render()
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif

		std::cout << "Render" << std::endl;
		map->ClearEntities();
		map->DrawEntities(*player);
		map->Draw();

		std::cout << static_cast<char>(player->getSymbol());

		for (int i = 0 ; i < enemies.size(); i++) 
		{
			std::cout << static_cast<char>(enemies[i]->getSymbol());
		}
	}


	void GameEngine::release()
	{
		std::cout << "Release" << std::endl;

	}
}
