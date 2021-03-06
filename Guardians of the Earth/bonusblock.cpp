#include "bonusblock.h"

cBonusBlock::cBonusBlock(b2World &physics_world, sf::Texture &texture, sf::Vector2f pos, short min_treasures, short max_treasures) :cBlock(physics_world, texture, pos)
{
	this->min_treasures = min_treasures;
	this->max_treasures = max_treasures;

	//BOX2D
	this->body->DestroyFixture(this->body->GetFixtureList());

	b2ChainShape shape;
	const float32 x = pos.x * 0.02f;
	const float32 y = pos.y * 0.02f;

	b2Vec2 vs[4];
	vs[0].Set(0.02f + x + 0.0f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[1].Set(x + 32 * 0.02f - 16 * 0.02f - 0.02f, y + 0.0f - 16 * 0.02f);
	vs[2].Set(x + 32 * 0.02f - 16 * 0.02f - 0.02f, y + 32 * 0.02f - 16 * 0.02f);
	vs[3].Set(0.02f + x + 0.0f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);

	shape.CreateLoop(vs, 4);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_BONUS_BLOCK);
	fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE) | CATEGORY(CAT_CHARACTER);
	fd.friction = 0.0f;
	this->body->CreateFixture(&fd);
}

void cBonusBlock::dropTreasures(b2World &physics_world, eWorld world_type, std::vector <cTreasure> &treasure, sf::Vector2f hit_force, bool *modulators)
{
	srand(time(NULL));

	for (int i = 0; i <= rand() % (this->max_treasures - this->min_treasures) + this->min_treasures; i++)
	{
		if (modulators[4])
		{
			hit_force.x = (float)(rand() % 200 - 100) / 10;
			hit_force.y = (float)(rand() % 200 - 100) / 10;
		}

		treasure.push_back(cTreasure(physics_world, world_type, sf::Vector2f(this->getPosition().x + rand() % 24 - (this->getOrigin().x - 4), this->getPosition().y + rand() % 24 - (this->getOrigin().y - 4)), true, hit_force.x, hit_force.y));
	}
}