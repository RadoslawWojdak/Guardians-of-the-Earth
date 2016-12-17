#include "bonusblock.h"

cBonusBlock::cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos) :cBlock(physics_world, texture, pos)
{
	min_treasures = 1;
	max_treasures = 4;
}

void cBonusBlock::dropTreasures(b2World *physics_world, eWorld world_type, std::vector <cTreasure> &treasure, sf::Vector2f hit_force)
{
	srand(time(NULL));

	for (int i = 0; i <= rand () % (max_treasures - min_treasures); i++)
		treasure.push_back(cTreasure(physics_world, world_type, sf::Vector2f(this->getPosition().x + rand() % 24 - (this->getOrigin().x - 4), this->getPosition().y + rand() % 24 - (this->getOrigin().y - 4)), true, hit_force.x, hit_force.y));
}