#pragma once
#include <vector>
#include "Entity.hpp"

namespace CPL
{
	class Map
	{
	public:
		Map();
		void Draw();

		void DrawEntities(const Entity& entity);
		void ClearEntities();

	private:
		const unsigned int width = 80;
		const unsigned int height = 25;
		 std::vector<std::vector<char>>tiles;
	};
}

