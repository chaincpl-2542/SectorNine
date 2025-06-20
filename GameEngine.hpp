#pragma once
#include <memory>
#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

namespace CPL 
{
	class GameEngine
	{
	public:
		GameEngine();
		void init();
		char handleInput();
		void update(char input);
		void render();
		void release();

		//Movement
		void moveUp(int y, int x);
		void moveDown(int y, int x);
		void moveLeft(int y, int x);
		void moveRight(int y, int x);
		void moveUpLeft(int y, int x);
		void moveUpRight(int y, int x);
		void moveDownLeft(int y, int x);
		void moveDownRight(int y, int x);
		void wait(int y, int x);

	private:
		std::unique_ptr<Map> map;
		std::unique_ptr<Player> player;
		std::vector<std::shared_ptr<Enemy>> enemies;

	};
}

