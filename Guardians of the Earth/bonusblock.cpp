#include "bonusblock.h"

cBonusBlock::cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(texture, pos);

	min_treasures = 1;
	max_treasures = 4;

	//BOX2D
	this->body = physics_world->CreateBody(&body_def);
	
	b2ChainShape shape;
	const float32 x = pos.x * 0.02f;
	const float32 y = pos.y * 0.02f;

	b2Vec2 vs[4];
	vs[0].Set(x + 0.0f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[1].Set(x + 32 * 0.02f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[2].Set(x + 32 * 0.02f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);
	vs[3].Set(x + 0.0f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);

	shape.CreateLoop(vs, 4);
	//shape.SetAsBox(0.02f * 32.0f / 2.0f, 0.02f * 32.0f / 2.0f, b2Vec2(x, y), 0.0f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_BONUS_BLOCK);
	fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE);
	fd.friction = 0.0f;
	this->body->CreateFixture(&fd);
}

/*
cBonusBlock::~cBonusBlock()
{
	sf::Vector2f pos = this->getPosition();

	srand(time(NULL));
	
	for (int i = 0; i <= rand() % (max_treasures - min_treasures); i++)
	{
		std::cout << pos.x << " ; " << pos.y << "   ";
		std::cout << treasure->size() << "\n";
		this->treasure->push_back(cTreasure(sf::Vector2f(15, 15), true, (float)(rand() % 41) / 10 - 2, 0));
	}
}
*/
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