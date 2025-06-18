#pragma once

namespace CPL 
{
	class Entity
	{
	private:
		int posX;
		int posY;
		char symbol;

	public:
		Entity(int x, int y, char symbol);
		int getX() const;
		int getY() const;
		int getSymbol() const;
	};
}

