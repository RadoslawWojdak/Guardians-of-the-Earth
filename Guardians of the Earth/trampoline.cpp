#include "trampoline.h"

cTrampoline::cTrampoline(b2World *physics_world, unsigned short id, sf::Vector2f pos, float force)
{
	this->setTextureRect(sf::IntRect(0, 20, 32, 12));
	this->adjustGraphicsParameters(t_object[id - 1], pos);
	this->move(0, 10);	//Przyci¹gniêcie do pod³o¿a (bêdzie le¿eæ tu¿ nad pod³o¿em (¿eby siê nie zlewa³o z pod³o¿em))

	this->force = force;

	//BOX2D
	float a = this->getTextureRect().width * 0.02f;
	float b = this->getTextureRect().height * 0.02f;

	body_def.type = b2_dynamicBody;
	body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
	body_def.fixedRotation = false;
	body_def.allowSleep = false;

	body = physics_world->CreateBody(&body_def);
	
	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 10.0f;
	fd.friction = 1.0f;
	fd.restitution = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_TRAMPOLINE);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK);

	body->CreateFixture(&fd);
}