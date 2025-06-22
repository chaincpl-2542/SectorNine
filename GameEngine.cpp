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

		for (int i = 0; i < enemies.size(); i++)
		{
			int x, y;
			do {
				x = rand() % (map->getWidth() - 2) + 1;
				y = rand() % (map->getHeight() - 2) + 1;
			} while (!map->isWalkable(x, y));

			enemies[i]->setPosition(x, y);
			map->DrawEntities(*enemies[i]);

		}


		map->DrawEntities(*player);
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
			case '8': move(0, -1); break;
			case '2': move(0, 1); break;
			case '4': move(-1, 0); break;
			case '6': move(1, 0); break;
			case '7': move(-1, -1); break;
			case '9': move(1, -1); break;
			case '1': move(-1, 1); break;
			case '3': move(1, 1); break;
			case '5': move(0, 0); break;
		}
	}

	void GameEngine::render()
	{
		ClearConsole();

		std::cout << "Render" << std::endl;
		map->ClearEntities();
		UpdateEnemyPosition();
		map->DrawEntities(*player);
		map->Draw();

		std::cout << static_cast<char>(player->getSymbol());

		for (int i = 0; i < enemies.size(); i++)
		{
			std::cout << static_cast<char>(enemies[i]->getSymbol());
		}
	}

	void GameEngine::UpdateEnemyPosition()
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			int x, y;
			x = enemies[i]->getX();
			y = enemies[i]->getY();

			enemies[i]->setPosition(x, y);
			map->DrawEntities(*enemies[i]);
		}
	}

	void GameEngine::release()
	{
		std::cout << "Release" << std::endl;

	}

#pragma region PlayerMovement

	void GameEngine::move(int dx, int dy)
	{
		int x = player->getX();
		int y = player->getY();
		tryMoveTo(x + dx, y + dy);
	}

	void GameEngine::tryMoveTo(int newX, int newY)
	{
		if (map->getTileType(newX, newY) == FLOOR)
			if (map->isWalkable(newX, newY))
				player->setPosition(newX, newY);
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
		std::cout << "===================== Control Map======================" << std::endl;
		std::cout << "7 8 9 \n4 5 6 \n1 2 3" << std::endl;
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
