#include "GameEngine.hpp"
#include "Player.hpp"
#include <iostream>
#include <conio.h>

namespace CPL
{
	GameEngine::GameEngine()
	{
		cfg.loadFromFile();
	}

	void GameEngine::init()
	{
		srand(static_cast<unsigned int>(time(nullptr)));
		std::cout << "Init\n";

		map = std::make_unique<Map>(cfg.mapWidth, cfg.mapHeight, cfg.doorChance);
		map->generateRoomsBSP();

		auto [px, py] = map->getPlayerStart();

		player = std::make_unique<Player>(cfg.playerHP);
		player->setPosition(px, py);
		map->DrawEntities(*player);

		map->UpdateVisibility(px, py, cfg.playerSight);

		enemies.clear();
		for (int i = 0; i < cfg.enemyAmount; ++i)
			enemies.push_back(std::make_shared<Enemy>());

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
		if (gameOver)
		{
			ClearConsole();

			std::cout << "Game Over\n"
				"=======\n"
				"Thank you for playing\n"
				"=======\n"
				"Make by Peeranat Luangaram and Olivia\n\n"
				"Press any key to restart the game.";
			return;
		}

		ClearConsole();

		map->ClearEntities();
		UpdateEnemyPosition();
		map->DrawEntities(*player);
		map->Draw();

		int maxHp = cfg.playerHP;
		std::cout << "\nHP: " << player->getHP() << "/" << maxHp << "  "
			<< "[F1] Show how to play\n";
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

		for (auto& enemy : enemies)
		{
			auto path = enemy->findPathToPlayer(*map,
				player->getX(), player->getY(), enemies);

			if (!path.empty())
			{
				int nextX = path[0].first;
				int nextY = path[0].second;

				if (nextX == player->getX() && nextY == player->getY())
				{
					player->takeDamage();
					std::cout << "Enemy hits you!  HP: "
						<< player->getHP() << "/12\n";

					if (player->isDead())
					{
						std::cout << "You died...\n";
						gameOver = true;
					}
				}
				else
				{
					enemy->setPosition(nextX, nextY);
				}
			}
			map->DrawEntities(*enemy);
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
		TileType tile = map->getTileType(newX, newY);

		if (tile == ESCAPE)
		{
			player->setPosition(newX, newY);
			gameWon = true;
			return true;
		}

		if (tile == FLOOR && map->isWalkable(newX, newY))
		{
			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);
			return true;
		}

		if (tile == PASS_KEY && map->isWalkable(newX, newY))
		{
			player->givePassKey();
			map->unlockAllLockedDoors();
			map->setTile(newX, newY, '.');
			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);
			return true;
		}

		if (map->getTile(newX, newY) == 'M')
		{
			player->giveMap();
			map->revealWholeDungeon();
			map->setTile(newX, newY, '.');
			std::cout << "You found a map!\n";
			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);
			return true;
		}

		if (tile == LOCKED_DOOR)
		{
			if (!map->isDoorOpen(newX, newY))
			{
				if (!player->hasPassKey())
				{
					std::cout << "Need keycard.\n";
					return false;
				}
				 map->setDoorOpen(newX, newY, true);
			}
			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);
			return true;
		}

		if (tile == MAP)
		{
			player->giveMap();
			map->revealWholeDungeon();
			map->setTile(newX, newY, '.');
			std::cout << "You found a map!\n";

			player->setPosition(newX, newY);
			map->UpdateVisibility(newX, newY);
			return true;
		}

		if (tile == 'L' && !map->isDoorOpen(newX, newY))
		{
			std::cout << "Need keycard.\n";
			return false;
		}

		return false;
	}


	void GameEngine::toggleDoorAroundPlayer()
	{
		int px = player->getX(), py = player->getY();
		const int dir[5][2] = { {0,0},{0,-1},{0,1},{-1,0},{1,0} };

		for (auto& d : dir)
		{
			int tx = px + d[0], ty = py + d[1];
			char tile = map->getTile(tx, ty);

			if (tile == '+') { map->toggleDoor(tx, ty); break; }
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

	void GameEngine::restart()
	{
		enemies.clear();
		map.reset();
		player.reset();

		gameOver = false;
		gameWon = false;

		init();
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
