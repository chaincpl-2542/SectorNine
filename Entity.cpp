#include "Entity.hpp"

namespace CPL 
{
	Entity::Entity(int x, int y, char symbol) : posX(x), posY(y), symbol(symbol)
	{

	};

	int Entity::getX() const
	{
		return posX;
	}

	int Entity::getY() const
	{
		return posY;
	}

	int Entity::getSymbol() const
	{
		return symbol;
	}

	void Entity::setPosition(int x, int y)
	{
		posX = x;
		posY = y;
	}
}