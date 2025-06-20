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
		const int F1 = 59; // F1 key

		std::cout << "Press F1 to see how to play." << std::endl;

		int input = _getch();

		if (input == 0 || input == 224)
		{
			int key = _getch();
			if (key == F1)
			{
				ShowHowToPlay();
				return ' ';
			}
			return ' ';
		}

		return static_cast<char>(input);
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
		ClearConsole();

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

	void GameEngine::ShowHowToPlay()
	{
		ClearConsole();
		std::cout << "How to play:" << std::endl;
		std::cout << "Use the numpad keys to move your character around the map." << std::endl;
		std::cout << "Avoid enemies and try to reach the goal!" << std::endl;
		std::cout << "\n" << std::endl;
		std::cout << "======================= Control =======================" << std::endl;
		std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
		std::cout << "7 = up-left, 8 = up, 9 = up-right" << std::endl;
		std::cout << "4 = left,    5 = wait, 6 = right" << std::endl;
		std::cout << "1 = down-left, 2 = down, 3 = down-right" << std::endl;
		std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
		std::cout << "=======================================================" <<std::endl;
		std::cout << "\n" << std::endl;
		std::cout << "Press 'x' to exit the game." << std::endl;
		std::cout << "\n" << std::endl;
		std::cout << "Press any key to continue..." << std::endl;

		_getch();
	}

#pragma endregion

	void GameEngine::ClearConsole()
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
	}
}
