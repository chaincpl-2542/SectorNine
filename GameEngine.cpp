#include "GameEngine.hpp"
#include "Player.hpp"
#include <iostream>
#include <conio.h>

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

		char input = _getch();
		return input;
	}

	void GameEngine::update(char input)
	{
		int x = player->getX();
		int y = player->getY();

		switch (input)
		{
			case '8': moveUp(y, x); break;
			case '2': moveDown(y, x); break;
			case '4': moveLeft(y, x); break;
			case '6': moveRight(y, x); break;
			case '7': moveUpLeft(y, x); break;
			case '9': moveUpRight(y, x); break;
			case '1': moveDownLeft(y, x); break;
			case '3': moveDownRight(y, x); break;
			case '5': wait(y, x); break;
			default:break;
		}
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

		for (int i = 0; i < enemies.size(); i++)
		{
			std::cout << static_cast<char>(enemies[i]->getSymbol());
		}
	}


	void GameEngine::release()
	{
		std::cout << "Release" << std::endl;

	}

#pragma region PlayerMovement

	void GameEngine::moveUp(int y, int x)
	{
		if (map->isWalkable(x, y - 1))
			player->setPosition(x, y - 1);
	}

	void GameEngine::moveDown(int y, int x)
	{
		if (map->isWalkable(x, y + 1))
			player->setPosition(x, y + 1);
	}

	void GameEngine::moveLeft(int y, int x)
	{
		if (map->isWalkable(x - 1, y))
			player->setPosition(x - 1, y);
	}

	void GameEngine::moveRight(int y, int x)
	{
		if (map->isWalkable(x + 1, y))
			player->setPosition(x + 1, y);
	}

	void GameEngine::moveUpLeft(int y, int x)
	{
		if (map->isWalkable(x - 1, y - 1))
			player->setPosition(x - 1, y - 1);
	}

	void GameEngine::moveUpRight(int y, int x)
	{
		if (map->isWalkable(x + 1, y - 1))
			player->setPosition(x + 1, y - 1);
	}

	void GameEngine::moveDownLeft(int y, int x)
	{
		if (map->isWalkable(x - 1, y + 1))
			player->setPosition(x - 1, y + 1);
	}

	void GameEngine::moveDownRight(int y, int x)
	{
		if (map->isWalkable(x + 1, y + 1))
			player->setPosition(x + 1, y + 1);
	}

	void GameEngine::wait(int y, int x)
	{
		player->setPosition(x, y);
	}

#pragma endregion
}
