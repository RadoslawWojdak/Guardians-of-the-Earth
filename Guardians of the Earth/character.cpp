#include "character.h"

cCharacter::cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(t_character[0], pos);

	this->last_speed.x = 0;
	this->last_speed.y = 1;

	this->immunity_time = 0;
	this->life = 3;
	this->score = 0;
	this->cash = 0;

	this->max_speed_x = 4.0f;
	this->last_position = this->getPosition();

	this->key.up = sf::Keyboard::Key::Up;
	this->key.down = sf::Keyboard::Key::Down;
	this->key.left = sf::Keyboard::Key::Left;
	this->key.right = sf::Keyboard::Key::Right;
	this->key.jump = sf::Keyboard::Key::Space;
	this->key.fire = sf::Keyboard::Key::LControl;

	this->is_immersed_in = FLUID_NONE;
	this->is_on_ice = false;
	this->is_on_ladder = false;
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
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.06f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_CHARACTER);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	this->body->CreateFixture(&fd);
}

void cCharacter::bodyRecreate(b2World &physics_world, eWorld world_type)
{
	//BOX2D
	float32 a = this->getTextureRect().width * 0.02f;
	float32 b = this->getTextureRect().height * 0.02f;

	body_def.position.Set(this->getPosition().x * 0.02f, this->getPosition().y * 0.02f);
	body_def.type = b2_dynamicBody;
	body_def.fixedRotation = true;
	body_def.allowSleep = false;

	this->body = physics_world.CreateBody(&body_def);

	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.06f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_CHARACTER);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	this->body->CreateFixture(&fd);
}

void cCharacter::initPet()
{
	this->pet_point = this->getPosition();
	this->pet = cPet(&(this->pet_point), 1);
}

void cCharacter::jump(float force)
{
	switch (this->is_immersed_in)
	{
	case FLUID_WATER: {this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force * pow(g_fluid_speed_multipler.water, 1.5))); break;}
	case FLUID_QUICKSAND: {this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force * pow(g_fluid_speed_multipler.quicksand, 1.5))); break;}
	default: {this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force)); break;}
	}

	this->can_jump = false;
}

void cCharacter::startInviolability()
{
	this->immunity_time = 120;
}

void cCharacter::immunityCountdown()
{
	if (this->isInviolability())
	{
		this->immunity_time--;

		if (this->immunity_time == 0)
			this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 255));
		else if (this->immunity_time < 75)
			this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a + 1));
		else if (this->getColor().a == 255)
			this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 127));
	}
}

void cCharacter::beenHit()
{
	if (this->isPetAlive())
	{
		this->pet.decreaseHP();
		this->startInviolability();
	}
	else
		this->kill();
}

void cCharacter::kill()
{
	if (!this->isDead())
	{
		this->life--;
		this->dead = true;
		this->pet.kill();

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
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x < 0)
					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			}
			else if (this->body->GetLinearVelocity().x < 0)
			{
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x > 0)
					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			}
		}
		if (sf::Keyboard::isKeyPressed(this->key.jump))
		{
			if (!stop_jump)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, this->body->GetLinearVelocity().y * 1.022f));
			if ((this->body->GetLinearVelocity().y == 0 && this->can_jump) || (this->body->GetLinearVelocity().y >= 0 && this->is_immersed_in != FLUID_NONE))	//1 - W przypadku gdy spadnie sk¹dœ (can_jump jest aktywne); 2 - W przypadku, gdy akurat prêdkoœæ Y by³aby równa 0 (miêdzy wyskokiem a upadkiem); 3 - W przypadku gdy obiekt jest zanurzony w p³ynie
			{
				this->jump(-5.0f);
				if (this->is_immersed_in == FLUID_NONE)
					this->stop_jump = false;
			}
		}
		else
			this->stop_jump = true;
	}
}

void cCharacter::specjalCollisions(b2World *physics_world, eWorld world_type, std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block)
{
	this->immunityCountdown();

	if (!this->isDead())
	{
		//Kolizje z Bonusowymi blokami
		bool *b_bs_to_destroy = new bool[bonus_block.size()];	//b_bs - bonus blocks
		for (unsigned short i = 0; i < bonus_block.size(); i++)
		{
			if (this->getGlobalBounds().intersects(bonus_block[i].getGlobalBounds()))
			{
				if (this->last_position.y - this->getOrigin().y >= bonus_block[i].getPosition().y + this->getOrigin().y)	//Je¿eli postaæ spad³a na NPC-a; last_position naprawia b³êdy zwi¹zane z œmierci¹ postaci, gdy spada³a zbyt szybko; +3 - gdy postaæ znajduje siê tu¿ nad NPC-em i chce na niego spaœæ (gracz nie chodi tu¿ nad pod³o¿em, lecz bezpoœrednio na nim)
					b_bs_to_destroy[i] = true;
				else
					b_bs_to_destroy[i] = false;
			}
			else
				b_bs_to_destroy[i] = false;
		}

		for (int i = bonus_block.size() - 1; i >= 0; i--)
			if (b_bs_to_destroy[i])
			{
				this->addStatsForBonusBlock();
				bonus_block[i].dropTreasures(physics_world, world_type, treasure, sf::Vector2f((float)((rand() % 2 ? -1 : 1) * rand() % 9) / 10.0f + this->last_speed.x * 2.25f, -(float)(rand() % 10 + 12) / 10.0f + this->last_speed.y * 2.25f));
				bonus_block[i].getBody()->GetWorld()->DestroyBody(bonus_block[i].getBody());
				bonus_block.erase(bonus_block.begin() + i);
			}

		delete[] b_bs_to_destroy;

		//Kolizje z NPC-ami
		bool *npcs_to_destroy = new bool[npc.size()];
		for (unsigned short i = 0; i < npc.size(); i++)
		{
			if (this->getGlobalBounds().intersects(npc[i].getGlobalBounds()))
			{
				if (this->last_position.y + this->getOrigin().y <= npc[i].getLastPosition().y - this->getOrigin().y + 3)	//Je¿eli postaæ spad³a na NPC-a; last_position naprawia b³êdy zwi¹zane z œmierci¹ postaci, gdy spada³a zbyt szybko; +3 - gdy postaæ znajduje siê tu¿ nad NPC-em i chce na niego spaœæ (gracz nie chodi tu¿ nad pod³o¿em, lecz bezpoœrednio na nim)
				{
					this->addStatsForNPC(npc[i]);
					npcs_to_destroy[i] = true;
					if (sf::Keyboard::isKeyPressed(this->key.jump))
					{
						this->jump(-5.0f);
						this->stop_jump = false;
					}
					else
						this->jump(-3.0f);
				}
				else	//Je¿eli dotkniêto NPC-a w inny sposób
				{
					if (!this->isInviolability())
						this->beenHit();
					npcs_to_destroy[i] = false;
				}
			}
			else
				npcs_to_destroy[i] = false;
		}

		for (int i = npc.size() - 1; i >= 0; i--)
			if (npcs_to_destroy[i])
				npc[i].kill();

		delete[] npcs_to_destroy;

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

		delete[] treasures_to_destroy;

		//Kolizje z lodem
		if (world_type == WORLD_ICE_LAND)
		{
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

		//Kolizje z trampolinami
		for (unsigned short i = 0; i < trampoline.size(); i++)
		{
			if (this->getGlobalBounds().intersects(trampoline[i].getGlobalBounds()))
			{
				if (sf::Keyboard::isKeyPressed(this->key.jump))
				{
					this->jump(-6.0f);
					this->stop_jump = false;
				}
				else
					this->jump(-4.0f);

				break;	//Nawet gdyby by³o wiêcej trampolin to nie robi³oby to ró¿nicy
			}
		}

		//Kolizje z drabinami
		bool ladder_collision = false;
		for (unsigned int i = 0; i < ladder.size(); i++)
		{
			if (this->getGlobalBounds().intersects(ladder[i].getGlobalBounds()))
			{
				ladder_collision = true;
				
				if (sf::Keyboard::isKeyPressed(this->key.up) || sf::Keyboard::isKeyPressed(this->key.down))
				{
					this->body->SetGravityScale(0.0f);
					this->is_on_ladder = true;
					this->stop_jump = true;

					if (sf::Keyboard::isKeyPressed(this->key.up) && sf::Keyboard::isKeyPressed(this->key.down))
						this->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					else if (sf::Keyboard::isKeyPressed(this->key.up))
						this->body->SetLinearVelocity(b2Vec2(0.0f, -1.75f));
					else if (sf::Keyboard::isKeyPressed(this->key.down))
						this->body->SetLinearVelocity(b2Vec2(0.0f, 1.75f));
				}
				
				if (this->is_on_ladder)
				{
					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));	//Dziêki temu postaæ bêdzie siê natychmiastowo zatrzymywaæ, gdy gracz póœci klawisz w bok (lewo lub prawo)

					if (!sf::Keyboard::isKeyPressed(this->key.up) && !sf::Keyboard::isKeyPressed(this->key.down))
						this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));
					
					if (sf::Keyboard::isKeyPressed(this->key.up) && sf::Keyboard::isKeyPressed(this->key.down))
						this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
					else if (sf::Keyboard::isKeyPressed(this->key.left))
						this->body->SetLinearVelocity(b2Vec2(-1.0f, this->body->GetLinearVelocity().y));
					else if (sf::Keyboard::isKeyPressed(this->key.right))
						this->body->SetLinearVelocity(b2Vec2(1.0f, this->body->GetLinearVelocity().y));
					
					if (sf::Keyboard::isKeyPressed(this->key.jump))
					{
						this->jump(-5.0f);
						this->stop_jump = false;
						this->is_on_ladder = false;
					}
				}

				break;	//Nawet gdyby by³o wiêcej drabin to nie robi³oby to ró¿nicy
			}
		}
		if (!ladder_collision)
			this->is_on_ladder = false;
	}
}

void cCharacter::applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size)
{
	if (!this->isDead() && !this->is_on_ladder)	//Gdy gracz jest na drabinie, nie dzia³a na niego ¿adna grawitacja
	{
		//Gracz nie mo¿e przyspieszyæ postaci, gdy ta spada
		if (this->body->GetLinearVelocity().y >= 0)
			this->stop_jump = true;

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
		this->pet.move();

		this->last_speed = this->body->GetLinearVelocity();
		this->last_position = this->getPosition();
		this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
		this->pet_point = this->getPosition();

		//Czy gracz wylecia³ poza ramkê poziomu
		if (this->getGlobalBounds().left < 0)
		{
			this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			this->setAllPositions(sf::Vector2f(this->getOrigin().x, this->getPosition().y));
		}
		if (this->getPosition().y - this->getOrigin().y > level_size.y)
			this->kill();
	}
}

void cCharacter::rebirth()
{
	this->last_speed.x = 0;
	this->last_speed.y = 1;

	this->immunity_time = 0;
	this->setColor(sf::Color(255, 255, 255, 255));

	this->is_immersed_in = FLUID_NONE;
	this->is_on_ice = false;
	this->is_on_ladder = false;

	this->pet_point = this->getPosition();
	this->pet.setPosition(this->pet_point);

	if (this->life > 0)
		this->dead = false;
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
	case -2:
	{
		this->pet.increaseHP();
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

void cCharacter::addStatsForBonusBlock()
{
	this->score += 25;
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
	this->last_position = this->getPosition();
	this->body->SetTransform(b2Vec2(pos.x * 0.02f, pos.y * 0.02f), 0);
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}

cPet cCharacter::getPet()
{
	return this->pet;
}

b2Body *cCharacter::getBody()
{
	return this->body;
}

bool cCharacter::hasLife()
{
	if (this->life == 0)
		return false;
	return true;
}

bool cCharacter::isPetAlive()
{
	if (this->pet.getHP() > 0)
		return true;
	return false;
}

bool cCharacter::isDead()
{
	return this->dead;
}

bool cCharacter::isInviolability()
{
	if (this->immunity_time > 0)
		return true;
	return false;
}