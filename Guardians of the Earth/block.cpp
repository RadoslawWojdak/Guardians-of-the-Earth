#include "block.h"

cBlock::cBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(texture, pos);

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
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_BLOCK);
	fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE) | CATEGORY(CAT_CHARACTER) | CATEGORY(CAT_BULLET);
	fd.friction = 0.0f;
	this->body->CreateFixture(&fd);
}

b2Body *cBlock::getBody()
{
	return this->body;
}