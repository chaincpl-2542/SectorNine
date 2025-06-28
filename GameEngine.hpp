#pragma once
#include <memory>
#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Config.hpp"

namespace CPL 
{
	class GameEngine
	{
	public:
		Config cfg;
		GameEngine();
		void init();
		char handleInput();
		void update(char input);
		void render();
		void release();

		//Movement
		void move(int dx, int dy);
		bool tryMoveTo(int newX, int newY);

		void toggleDoorAroundPlayer();

		//How to play
		void ShowHowToPlay();

		//Clear console
		void ClearConsole();

		//Update enemy
		void UpdateEnemyPosition();

		void restart();
		bool isGameOver() const { return gameOver; }

		bool gameWon = false;

	private:
		bool gameOver = false;
		std::unique_ptr<Map> map;
		std::unique_ptr<Player> player;
		std::vector<std::shared_ptr<Enemy>> enemies;

	};
}

