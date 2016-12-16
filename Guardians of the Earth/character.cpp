#include "character.h"

cCharacter::cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(t_character[0], pos);

	this->last_speed.x = 0;
	this->last_speed.y = 1;
	
	//BOX2D
	float32 a = this->getTextureRect().width * 0.02f;
	float32 b = this->getTextureRect().height * 0.02f;

	body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
	body_def.type = b2_dynamicBody;
	body_def.fixedRotation = true;
	body_def.allowSleep = false;

	this->body = physics_world->CreateBody(&body_def);
	
	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.03f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_CHARACTER);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	this->body->CreateFixture(&fd);
}

void cCharacter::control()
{
	if (this->last_speed.y >= 0 && this->body->GetLinearVelocity().y <= 0)
		this->can_jump = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->setScale(1.0f, 1.0f);

		this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f, this->body->GetLinearVelocity().y));
		if (this->body->GetLinearVelocity().x > 4.0f)
			this->body->SetLinearVelocity(b2Vec2(4.0f, this->body->GetLinearVelocity().y));
		else if (this->body->GetLinearVelocity().x < 0)
			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.2f, this->body->GetLinearVelocity().y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->setScale(-1.0f, 1.0f);

		this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f, this->body->GetLinearVelocity().y));
		if (this->body->GetLinearVelocity().x < -4.0f)
			this->body->SetLinearVelocity(b2Vec2(-4.0f, this->body->GetLinearVelocity().y));
		else if (this->body->GetLinearVelocity().x > 0)
			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.2f, this->body->GetLinearVelocity().y));
	}
	//Gdy nie jest naciœniêty ¿aden z klawiszy (lewo, prawo), to postaæ zaczyna siê zatrzymywaæ
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (this->body->GetLinearVelocity().x > 0)
		{
			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f, this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x < 0)
				this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
		}
		else if (this->body->GetLinearVelocity().x < 0)
		{
			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f, this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x > 0)
				this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (this->body->GetLinearVelocity().y == 0 && this->can_jump)	//1 - W przypadku gdy spadnie sk¹dœ (can_jump jest aktywne); 2 - W przypadku, gdy akurat prêdkoœæ Y by³aby równa 0 (miêdzy wyskokiem a upadkiem)
		{
			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, this->body->GetLinearVelocity().y - 7.0f));
			this->can_jump = false;
		}
	}

	this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
	this->last_speed = this->body->GetLinearVelocity();
}

void cCharacter::specjalCollisions(std::vector <cTreasure> &treasure)
{
	//Kolizje ze skarbami
	bool *treasures_to_destroy = new bool[treasure.size()];
	for (unsigned short i = 0; i < treasure.size(); i++)
	{
		if (this->getGlobalBounds().intersects(treasure[i].getGlobalBounds()))
		{
			this->addStatsForTreasure(treasure[i]);
			treasures_to_destroy[i] = true;
		}
		else
			treasures_to_destroy[i] = false;
	}

	for (unsigned short i = 0; i < treasure.size(); i++)
		if (treasures_to_destroy[i])
			treasure.erase(treasure.begin() + i);
}

void cCharacter::addStatsForTreasure(cTreasure &treasure)
{
	switch (treasure.getValue())
	{
	case -1:
	{
		this->life++;
	}
	default:
	{
		this->score += 10;
		this->cash += treasure.getValue();
	}
	}
}

void cCharacter::setAllPositions(sf::Vector2f pos)
{
	this->body->SetTransform(b2Vec2(pos.x * 0.02f, pos.y * 0.02f), 0);
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}