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
		srand(static_cast<unsigned int>(time(nullptr)));
		std::cout << "Init\n";

		map = std::make_unique<Map>();
		map->generateRoomsBSP();

		auto [px, py] = map->getPlayerStart();

		player = std::make_unique<Player>();
		player->setPosition(px, py);
		map->DrawEntities(*player);

		map->UpdateVisibility(px, py, 5);

		map->ShowStaticMapOnly();

		enemies.clear();
		enemies.push_back(std::make_shared<Enemy>());
		//enemies.push_back(std::make_shared<Enemy>());
		//enemies.push_back(std::make_shared<Enemy>());

		const int MIN_DIST = 10;
		for (auto& enemy : enemies)
		{
			int ex, ey;
			do {
				ex = rand() % (map->getWidth() - 2) + 1;
				ey = rand() % (map->getHeight() - 2) + 1;
			} while (
				map->getTileType(ex, ey) != FLOOR
				|| std::abs(ex - px) + std::abs(ey - py) < MIN_DIST);

			enemy->setPosition(ex, ey);
			map->DrawEntities(*enemy);
		}

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
			return static_cast<char>(input);
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
			case 'o': toggleDoorAroundPlayer(); break;
			case '0': toggleDoorAroundPlayer(); break;
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

	void Map::UpdateVisibility(int px, int py, int radius = 5)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// Reset visibility
				visible[y][x] = false;
			}
		}

		for (int dy = -radius; dy <= radius; ++dy)
		{
			for (int dx = -radius; dx <= radius; ++dx)
			{
				int nx = px + dx;
				int ny = py + dy;
				if (nx >= 0 && nx < width && ny >= 0 && ny < height)
				{
					if (abs(dx) + abs(dy) <= radius) // ข้าวหลามตัด
					{
						visible[ny][nx] = true;
						revealed[ny][nx] = true;
					}
				}
			}
		}
	}

	void GameEngine::UpdateEnemyPosition()
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			auto path = enemies[i]->findPathToPlayer(*map, player->getX(), player->getY(), enemies);
			if (!path.empty()) {
				int dx = std::abs(enemies[i]->getX() - player->getX());
				int dy = std::abs(enemies[i]->getY() - player->getY());

				if (dx > 1 || dy > 1) {
					int nextX = path[0].first;
					int nextY = path[0].second;
					enemies[i]->setPosition(nextX, nextY);
				}
			}

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
		tryMoveTo(player->getX() + dx, player->getY() + dy);
	}

	bool GameEngine::tryMoveTo(int newX, int newY)
	{
		if (map->getTileType(newX, newY) == ESCAPE)
		{
			player->setPosition(newX, newY);
			gameWon = true;
			return true;

		}
		if (map->getTileType(newX, newY) == FLOOR &&
			map->isWalkable(newX, newY))
		{
			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);

			return true;
		}
		return false;
	}

	void GameEngine::toggleDoorAroundPlayer()
	{
		int px = player->getX();
		int py = player->getY();

		const int dir[5][2] = { {0,0}, {0,-1}, {0,1}, {-1,0}, {1,0} };

		for (auto& d : dir)
		{
			int tx = px + d[0];
			int ty = py + d[1];

			if (map->isDoor(tx, ty)) {
				map->toggleDoor(tx, ty);
				break;
			}
		}
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
