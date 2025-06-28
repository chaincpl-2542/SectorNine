#pragma once
#include "Entity.hpp"

namespace CPL 
{
	class Player : public Entity
	{
		int hp = 12;

	public:

		Player(int hp);
		bool hasPassKey() const { return m_hasPassKey; }
		void clearPassKey() { m_hasPassKey = false; }
		void givePassKey() { m_hasPassKey = true; }
		void giveMap() { hasMap_ = true; }
		int  getHP()   const { return hp; }
		bool isDead()  const { return hp <= 0; }
		void takeDamage(int dmg = 1) { hp -= dmg; }

	private:

		bool m_hasPassKey = false;
		bool hasMap_ = false;
	};
}

