#include "bonusblock.h"

cBonusBlock::cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos) :cBlock(physics_world, texture, pos)
{
	min_treasures = 1;
	max_treasures = 4;
}

void cBonusBlock::dropTreasures(b2World *physics_world, eWorld world_type, std::vector <cTreasure> &treasure)
{
	srand(time(NULL));

	for (int i = 0; i <= rand () % (max_treasures - min_treasures); i++)
		treasure.push_back(cTreasure(physics_world, world_type, sf::Vector2f(this->getPosition().x + rand() % 32, this->getPosition().y + rand() % 32), true, (float)((rand() % 2 ? -1 : 1) * rand() % 41 + 9) / 10, (float)((rand() % 2 ? -1 : 1) * rand() % 41 + 9) / 10));
}

b2Body* cBonusBlock::getBody()
{
	return this->body;
}