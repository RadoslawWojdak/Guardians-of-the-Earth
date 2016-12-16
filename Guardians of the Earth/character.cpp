#include "character.h"

#include <iostream>

cCharacter::cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(t_character[0], pos);

	this->last_speed.x = 0;
	this->last_speed.y = 1;

	this->life = 3;
	this->score = 0;
	this->cash = 0;

	this->max_speed_x = 4.0f;

	this->key.up = sf::Keyboard::Key::Up;
	this->key.down = sf::Keyboard::Key::Down;
	this->key.left = sf::Keyboard::Key::Left;
	this->key.right = sf::Keyboard::Key::Right;
	this->key.jump = sf::Keyboard::Key::Space;
	this->key.fire = sf::Keyboard::Key::LControl;

	this->is_immersed_in = FLUID_NONE;
	this->is_on_ice = false;
	this->dead = false;

	//BOX2D
	float32 a = this->getTextureRect().width * 0.02f;
	float32 b = this->getTextureRect().height * 0.02f;

	body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
	body_def.type = b2_dynamicBody;
	body_def.fixedRotation = true;
	body_def.allowSleep = false;

	this->body = physics_world->CreateBody(&body_def);
	
	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.04f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_CHARACTER);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	this->body->CreateFixture(&fd);
}

void cCharacter::jump(float force)
{
	float speed_multipler = 1;
	switch (this->is_immersed_in)
	{
	case FLUID_WATER: {speed_multipler = g_fluid_speed_multipler.water; break;}
	case FLUID_QUICKSAND: {speed_multipler = g_fluid_speed_multipler.quicksand; break;}
	}

	this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force * pow(speed_multipler, 1.5)));
	this->can_jump = false;
}

void cCharacter::kill()
{
	if (!this->isDead())
	{
		this->life--;
		this->dead = true;

		this->body->GetWorld()->DestroyBody(this->body);
	}
}

void cCharacter::control()
{
	if (!this->isDead())
	{
		float speed_multipler = 1;
		switch (this->is_immersed_in)
		{
		case FLUID_WATER: {speed_multipler = g_fluid_speed_multipler.water; break;}
		case FLUID_QUICKSAND: {speed_multipler = g_fluid_speed_multipler.quicksand; break;}
		}

		if (this->last_speed.y >= 0 && this->body->GetLinearVelocity().y <= 0)
			this->can_jump = true;

		if (sf::Keyboard::isKeyPressed(this->key.right))
		{
			this->setScale(1.0f, 1.0f);

			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x > max_speed_x * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2(max_speed_x * speed_multipler, this->body->GetLinearVelocity().y));
			else if (this->body->GetLinearVelocity().x < 0)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
		}
		if (sf::Keyboard::isKeyPressed(this->key.left))
		{
			this->setScale(-1.0f, 1.0f);

			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x < -max_speed_x * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2(-max_speed_x * speed_multipler, this->body->GetLinearVelocity().y));
			else if (this->body->GetLinearVelocity().x > 0)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
		}
		//Gdy nie jest naciœniêty ¿aden z klawiszy (lewo, prawo), to postaæ zaczyna siê zatrzymywaæ
		if (!sf::Keyboard::isKeyPressed(this->key.right) && !sf::Keyboard::isKeyPressed(this->key.left))
		{
			if (this->body->GetLinearVelocity().x > 0)
			{
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f / speed_multipler, this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x < 0)
					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			}
			else if (this->body->GetLinearVelocity().x < 0)
			{
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f / speed_multipler, this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x > 0)
					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			}
		}
		if (sf::Keyboard::isKeyPressed(this->key.jump))
		{
			if ((this->body->GetLinearVelocity().y == 0 && this->can_jump) || (this->body->GetLinearVelocity().y >= 0 && this->is_immersed_in != FLUID_NONE))	//1 - W przypadku gdy spadnie sk¹dœ (can_jump jest aktywne); 2 - W przypadku, gdy akurat prêdkoœæ Y by³aby równa 0 (miêdzy wyskokiem a upadkiem); 3 - W przypadku gdy obiekt jest zanurzony w p³ynie
				this->jump(-7.0f);
		}
	}
}

void cCharacter::specjalCollisions(std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid)
{
	if (!this->isDead())
	{
		//Kolizje z NPC-ami
		bool *npcs_to_destroy = new bool[npc.size()];
		for (unsigned short i = 0; i < npc.size(); i++)
		{
			if (this->getGlobalBounds().intersects(npc[i].getGlobalBounds()))
			{
				if (this->getPosition().y + this->getOrigin().y <= npc[i].getGlobalBounds().top + 6)	//Je¿eli postaæ spad³a na NPC-a
				{
					this->addStatsForNPC(npc[i]);
					npcs_to_destroy[i] = true;
					this->jump(-3.0f);
				}
				else	//Je¿eli dotkniêto NPC-a w inny sposób
				{
					this->kill();
					npcs_to_destroy[i] = false;
				}
			}
			else
				npcs_to_destroy[i] = false;
		}

		for (int i = npc.size() - 1; i >= 0; i--)
			if (npcs_to_destroy[i])
				npc[i].kill();

		if (this->isDead())
			return;

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

		for (int i = treasure.size() - 1; i >= 0; i--)
			if (treasures_to_destroy[i])
				treasure.erase(treasure.begin() + i);

		//Kolizje z lodem
		if (this->can_jump)	//Dopiero jak stanie na czymœ innym ni¿ lód, to przestaje siê œlizgaæ
			this->is_on_ice = false;
		for (unsigned short i = 0; i < fluid.size(); i++)
		{
			if (this->getGlobalBounds().intersects(fluid[i].getGlobalBounds()))
			{
				this->is_on_ice = true;
				break;
			}
		}
	}
}

void cCharacter::applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size)
{
	if (!this->isDead())
	{
		//Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)
		if (this->isCollisionOnGrid(fluid, grid_size))
		{
			switch (world_type)
			{
			case WORLD_DESERT:
			{
				this->is_immersed_in = FLUID_QUICKSAND;

				this->body->SetGravityScale(0.035f);
				if (this->body->GetLinearVelocity().y > 0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.5f));
				else if (this->body->GetLinearVelocity().y < -0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -0.5f));

				break;
			}
			default:
			{
				this->is_immersed_in = FLUID_WATER;

				this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 2.0f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 2.0f));
				else if (this->body->GetLinearVelocity().y < -2.0f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -2.0f));

				break;
			}
			}
		}
		else
		{
			if (this->is_immersed_in != FLUID_NONE && sf::Keyboard::isKeyPressed(this->key.jump))	//Je¿eli wyskoczy³ z wody
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -5.0f));

			this->body->SetGravityScale(1.0f);
			this->is_immersed_in = FLUID_NONE;
		}
	}
}

void cCharacter::move(sf::Vector2f level_size)
{
	if (!this->isDead())
	{
		this->last_speed = this->body->GetLinearVelocity();
		this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);

		//Czy gracz wylecia³ poza ramkê poziomu
		if (this->getGlobalBounds().left < 0)
		{
			this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			this->setAllPositions(sf::Vector2f(this->getOrigin().x, this->getPosition().y));
		}
		else if (this->getPosition().x + this->getOrigin().x > level_size.x)
		{
			this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			this->setAllPositions(sf::Vector2f(level_size.x - this->getOrigin().x, this->getPosition().y));
		}
		if (this->getPosition().y - this->getOrigin().y > level_size.y)
			this->kill();
	}
}

void cCharacter::addStatsForTreasure(cTreasure &treasure)
{
	switch (treasure.getValue())
	{
	case -1:
	{
		this->life++;
		break;
	}
	default:
	{
		this->score += 10;
		this->cash += treasure.getValue();
		break;
	}
	}
}

void cCharacter::addStatsForNPC(cNPC &npc)
{
	this->score += 50;
}

void cCharacter::drawStats(sf::RenderWindow &win, sf::Vector2f left_top_corner)
{
	left_top_corner.x += win.getView().getCenter().x - win.getSize().x / 2;
	left_top_corner.y += win.getView().getCenter().y - win.getSize().y / 2;

	sf::String text_str(L"SCORE: ");
	std::string nr;
	std::stringstream ss;
	ss << this->score;
	nr = ss.str();
	ss.str("");

	text_str += nr;
	sf::Text text(text_str, font, 22);
	text.setPosition(left_top_corner);
	if (this->isDead())
		text.setFillColor(sf::Color(127, 127, 127));
	win.draw(text);


	text_str.clear();
	text_str = L"LIFE: ";
	ss << this->life;
	nr = ss.str();
	ss.str("");

	text_str += nr;
	text.setString(text_str);
	left_top_corner.y += 16;
	text.setPosition(left_top_corner);
	win.draw(text);


	text_str.clear();
	text_str = L"CASH: ";
	ss << this->cash;
	nr = ss.str();
	ss.clear();

	text_str += nr;
	text.setString(text_str);
	left_top_corner.y += 16;
	text.setPosition(left_top_corner);
	win.draw(text);
}

void cCharacter::setAllPositions(sf::Vector2f pos)
{
	this->body->SetTransform(b2Vec2(pos.x * 0.02f, pos.y * 0.02f), 0);
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}

bool cCharacter::isDead()
{
	return this->dead;
}