#include "character.h"

sControlKeys g_player_control_keys[4];

void cCharacter::initControlKeys(short player_no)
{
	switch (player_no)
	{
	case 1:
	{
		this->key.is_pad = false;
		this->key.pad = -1;

		this->key.up.key = sf::Keyboard::Key::Up;
		this->key.down.key = sf::Keyboard::Key::Down;
		this->key.left.key = sf::Keyboard::Key::Left;
		this->key.right.key = sf::Keyboard::Key::Right;
		this->key.jump.key = sf::Keyboard::Key::Space;
		this->key.fire.key = sf::Keyboard::Key::LControl;
		this->key.special1.key = sf::Keyboard::Key::RShift;
		break;
	}
	case 2:
	{
		this->key.is_pad = false;
		this->key.pad = -1;

		this->key.up.key = sf::Keyboard::Key::W;
		this->key.down.key = sf::Keyboard::Key::S;
		this->key.left.key = sf::Keyboard::Key::A;
		this->key.right.key = sf::Keyboard::Key::D;
		this->key.jump.key = sf::Keyboard::Key::E;
		this->key.fire.key = sf::Keyboard::Key::Q;
		this->key.special1.key = sf::Keyboard::Key::F;
		break;
	}
	case 3:
	{
		if (sf::Joystick::isConnected(0))
		{
			this->key.is_pad = true;
			this->key.pad = 0;

			this->key.jump.button = 2;
			this->key.fire.button = 3;
			this->key.special1.button = 5;
		}
		else
		{
			this->key.is_pad = false;
			this->key.pad = -1;

			this->key.up.key = sf::Keyboard::Key::U;
			this->key.down.key = sf::Keyboard::Key::J;
			this->key.left.key = sf::Keyboard::Key::H;
			this->key.right.key = sf::Keyboard::Key::K;
			this->key.jump.key = sf::Keyboard::Key::I;
			this->key.fire.key = sf::Keyboard::Key::Y;
			this->key.special1.key = sf::Keyboard::Key::L;
		}
		break;
	}
	case 4:
	{
		if (sf::Joystick::isConnected(1))
		{
			this->key.is_pad = true;
			this->key.pad = 1;

			this->key.jump.button = 2;
			this->key.fire.button = 3;
			this->key.special1.button = 5;
		}
		else
		{
			this->key.is_pad = false;
			this->key.pad = -1;

			this->key.up.key = sf::Keyboard::Key::Numpad8;
			this->key.down.key = sf::Keyboard::Key::Numpad2;
			this->key.left.key = sf::Keyboard::Key::Numpad4;
			this->key.right.key = sf::Keyboard::Key::Numpad6;
			this->key.jump.key = sf::Keyboard::Key::Numpad9;
			this->key.fire.key = sf::Keyboard::Key::Numpad7;
			this->key.special1.key = sf::Keyboard::Key::Numpad1;
		}
		break;
	}
	}
}

cCharacter::cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacterAnimation(t_character[0], pos)
{
	this->animationStanding();
	
	this->player_no = player_no;

	this->exp_bar.setColor(sf::Color(this->heart.getColor().g, this->heart.getColor().b, this->heart.getColor().a, 192));
	this->exp_bar.setTexture(t_experience_bar);
	this->exp_bar.setTextureRect(sf::IntRect(0, 0, 0, 0));

	this->last_speed.x = 0;
	this->last_speed.y = 1;

	this->stats_window.setTexture(t_stats_window);
	this->stats_window.setColor(sf::Color(this->stats_window.getColor().g, this->stats_window.getColor().b, this->stats_window.getColor().a, 192));
	this->heart.setTexture(t_heart);
	this->heart.setColor(sf::Color(this->heart.getColor().g, this->heart.getColor().b, this->heart.getColor().a, 192));
	
	this->immunity_time = 0;
	this->special1_time = 0;
	this->life = 3;
	this->score = 0;
	this->cash = 0;

	this->max_speed_x = 4.5f;
	this->extra_jump = 0;
	if (modulators[1])
		this->extra_jump++;
	this->last_position = this->getPosition();

	this->initControlKeys(player_no);

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
	this->pet_point = sf::Vector2f(this->getPosition().x, this->getPosition().y + 24);
	this->pet = cPet(this->character_type, &(this->pet_point), 1);
}

void cCharacter::jump(float force)
{
	switch (this->is_immersed_in)
	{
	case FLUID_WATER: {this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force * pow(g_fluid_speed_multipler.water, 1.5))); break;}
	case FLUID_QUICKSAND: {this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force * pow(g_fluid_speed_multipler.quicksand, 1.5))); break;}
	default: 
	{
		if (this->body->GetLinearVelocity().y > 0 && this->possible_extra_jumps > 0)
			this->possible_extra_jumps--;

		this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, force));
		break;
	}
	}

	this->can_jump = false;
}

void cCharacter::levelUp()
{
	this->exp -= this->requiredExpToLevelUp();
	this->lvl++;
	this->skill_points++;
}

void cCharacter::addPassiveSkill(unsigned short skill_id)
{
	;
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

void cCharacter::control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet)
{
	;	//Funkcja wirtualna
}

void cCharacter::specialCollisions(b2World *physics_world, eWorld world_type, bool *modulators, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block)
{
	if (!this->isDead())
	{
		//Kolizje z Bonusowymi blokami
		for (int i = bonus_block.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(bonus_block[i].getGlobalBounds()))
			{
				if (this->last_position.y - this->getOrigin().y >= bonus_block[i].getPosition().y + this->getOrigin().y || this->isSpecial1())
				{
					this->addStatsForBonusBlock();
					bonus_block[i].dropTreasures(physics_world, world_type, treasure, sf::Vector2f((float)((rand() % 2 ? -1 : 1) * rand() % 9) / 10.0f + this->last_speed.x * 2.25f, -(float)(rand() % 10 + 12) / 10.0f + this->last_speed.y * 2.25f), modulators);
					bonus_block[i].getBody()->GetWorld()->DestroyBody(bonus_block[i].getBody());
					bonus_block.erase(bonus_block.begin() + i);
				}
			}
		}

		//Kolizje z NPC-ami
		for (int i = npc.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(npc[i].getGlobalBounds()))
			{
				if ((this->last_position.y + this->getOrigin().y <= npc[i].getLastPosition().y - this->getOrigin().y + 3 && (!npc[i].getFeatures().top_hurts || this->isSpecial1())))	//Je¿eli postaæ spad³a na NPC-a; last_position naprawia b³êdy zwi¹zane z œmierci¹ postaci, gdy spada³a zbyt szybko; +3 - gdy postaæ znajduje siê tu¿ nad NPC-em i chce na niego spaœæ (gracz nie chodi tu¿ nad pod³o¿em, lecz bezpoœrednio na nim)
				{
					this->setAllPositions(sf::Vector2f(this->getPosition().x, npc[i].getGlobalBounds().top - this->getTextureRect().height / 2 - 4));

					if (this->can_crush)
					{
						npc[i].hurt(1.0f);
						if (npc[i].isDead())
						{
							this->addStatsForNPC(npc[i]);
							npc.erase(npc.begin() + i);
						}
					}

					if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button)))
					{
						this->jump(-5.0f);
						this->stop_jump = false;
					}
					else
						this->jump(-3.0f);

					this->possible_extra_jumps = this->extra_jump;
				}
				else	//Je¿eli dotkniêto NPC-a w inny sposób
				{
					if (!this->isSpecial1())
					{
						if (!this->isInviolability())
							this->beenHit();
					}
					else
					{
						npc[i].hurt(0.1f);
						if (npc[i].isDead())
						{
							this->addStatsForNPC(npc[i]);
							npc.erase(npc.begin() + i);
						}
					}
				}
			}
		}

		if (this->isDead())
			return;

		//Kolizje z power-up'ami
		for (int i = power_up.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(power_up[i].getGlobalBounds()))
			{
				this->addStatsForPowerUp(power_up[i]);
				power_up.erase(power_up.begin() + i);
			}
		}

		//Kolizje ze skarbami
		for (int i = treasure.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(treasure[i].getGlobalBounds()))
			{
				this->addStatsForTreasure(treasure[i]);
				treasure.erase(treasure.begin() + i);
			}
		}

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
				if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button)))
				{
					this->jump(-6.0f);
					this->stop_jump = false;
				}
				else
					this->jump(-4.0f);

				this->possible_extra_jumps = this->extra_jump;
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
				
				if ((!this->key.is_pad && (sf::Keyboard::isKeyPressed(this->key.up.key) || sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < -1.0f || sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f)))
				{
					this->dir = DIR_UP;

					this->body->SetGravityScale(0.0f);
					this->is_on_ladder = true;
					this->stop_jump = true;
					this->can_jump = true;

					if ((!this->key.is_pad && (sf::Keyboard::isKeyPressed(this->key.up.key) && sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < -1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.up.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < -1.0f))
					{
						this->dir = DIR_UP;
						this->body->SetLinearVelocity(b2Vec2(0.0f, -1.75f));
						if (!this->isSpecial1())
							this->animationClimbing(true);
					}
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.down.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f))
					{
						this->dir = DIR_DOWN;
						this->body->SetLinearVelocity(b2Vec2(0.0f, 1.75f));
						if (!this->isSpecial1())
							this->animationClimbing(false);
					}
				}
				
				if (this->is_on_ladder)
				{
					this->possible_extra_jumps = this->extra_jump;

					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));	//Dziêki temu postaæ bêdzie siê natychmiastowo zatrzymywaæ, gdy gracz póœci klawisz w bok (lewo lub prawo)

					if ((!this->key.is_pad && (!sf::Keyboard::isKeyPressed(this->key.up.key) && !sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > -1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));

					if ((!this->key.is_pad && (sf::Keyboard::isKeyPressed(this->key.up.key) && sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < -1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.left.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) < -1.0f))
					{
						this->dir = DIR_LEFT;
						this->body->SetLinearVelocity(b2Vec2(-1.0f, this->body->GetLinearVelocity().y));
					}
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.right.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) > 1.0f))
					{
						this->dir = DIR_RIGHT;
						this->body->SetLinearVelocity(b2Vec2(1.0f, this->body->GetLinearVelocity().y));
					}

					if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button)))
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

			if (world_type != WORLD_ICE_LAND)
				this->possible_extra_jumps = this->extra_jump;
		}
		else
		{
			if (this->is_immersed_in != FLUID_NONE && ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button))))	//Je¿eli wyskoczy³ z wody
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -5.0f));

			this->body->SetGravityScale(1.0f);
			this->is_immersed_in = FLUID_NONE;
		}
	}
}

void cCharacter::move(sf::RenderWindow &win, sf::Vector2f level_size)
{
	if (!this->isDead())
	{
		this->pet.move();

		this->last_speed = this->body->GetLinearVelocity();
		this->last_position = this->getPosition();
		this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
		this->pet_point = sf::Vector2f(this->getPosition().x, this->getPosition().y + 24);
		
		//Czy gracz wylecia³ poza ramkê poziomu
		if (this->getGlobalBounds().left < win.getView().getCenter().x - win.getView().getSize().x / 2)
		{
			this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			this->setAllPositions(sf::Vector2f(win.getView().getCenter().x - win.getView().getSize().x / 2 + this->getOrigin().x, this->getPosition().y));
		}
		else if (this->getGlobalBounds().left + this->getGlobalBounds().width > win.getView().getCenter().x + win.getView().getSize().x / 2 && win.getView().getCenter().x + win.getView().getSize().x / 2 < level_size.x)
		{
			this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
			this->setAllPositions(sf::Vector2f(win.getView().getCenter().x + win.getView().getSize().x / 2 - this->getOrigin().x, this->getPosition().y));
		}
		if (this->getPosition().y - this->getOrigin().y > level_size.y)
			this->kill();
	}
}

void cCharacter::checkIndicators(b2World *world, eWorld world_type, std::vector <cBullet> &bullet)
{
	;
}

void cCharacter::rebirth()
{
	this->dir = DIR_RIGHT;

	this->last_speed.x = 0;
	this->last_speed.y = 1;
	this->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

	this->immunity_time = 0;
	this->special1_time = 0;
	this->setColor(sf::Color(255, 255, 255, 255));

	this->is_immersed_in = FLUID_NONE;
	this->is_on_ice = false;
	this->is_on_ladder = false;

	this->pet_point = sf::Vector2f(this->getPosition().x, this->getPosition().y + 24);
	this->pet.setPosition(this->pet_point);

	if (this->life > 0)
		this->dead = false;
}

void cCharacter::addHP()
{
	this->pet.increaseHP();
}

void cCharacter::addLife()
{
	this->life++;
}

void cCharacter::addPower(short power_id)
{
	;
}

void cCharacter::addStatsForPowerUp(cPowerUp &power_up)
{
	switch (power_up.getPower())
	{
	case 1: {this->addPower(1); this->score += 100 * g_score_multipler; break;}	//+10 pocisków
	case 2: {this->addPower(2); this->score += 100 * g_score_multipler; break;}	//+1 special1
	}
}

void cCharacter::addStatsForTreasure(cTreasure &treasure)
{
	switch (treasure.getValue())
	{
	case -1:
	{
		this->addLife();
		break;
	}
	case -2:
	{
		this->addHP();
		break;
	}
	default:
	{
		this->score += 10 * g_score_multipler;
		this->cash += treasure.getValue();
		break;
	}
	}
}

void cCharacter::addStatsForNPC(cNPC &npc)
{
	this->score += 50 * g_score_multipler;
	this->exp += npc.getExperience();
}

void cCharacter::addStatsForBonusBlock()
{
	this->score += 25 * g_score_multipler;
}

void cCharacter::addStatsForEndOfLevel(unsigned int level_number, unsigned short experience_countdown)
{
	this->exp += experience_countdown * 0.075f;
	while (this->exp > this->requiredExpToLevelUp())
		this->levelUp();
}

void cCharacter::addSkill(unsigned short skill_id)
{
	this->number_of_skill[skill_id - 1]++;
	this->skill_points--;

	addPassiveSkill(skill_id - 1);
}

void cCharacter::subtractCash(unsigned int how_many_to_subtract)
{
	this->cash -= how_many_to_subtract;
}

void cCharacter::drawStats(sf::RenderWindow &win, sf::Vector2f left_top_corner)
{
	left_top_corner.x += win.getView().getCenter().x - win.getSize().x / 2;
	left_top_corner.y += win.getView().getCenter().y - win.getSize().y / 2;

	this->stats_window.setPosition(left_top_corner);
	win.draw(stats_window);

	//Punkty
	sf::String text_str;
	std::string number;
	std::stringstream ss;
	ss << this->score;
	number = ss.str();
	ss.str("");

	text_str = number;
	sf::Text text(text_str, font[1], 20);
	text.setFillColor(sf::Color(255, 255, 255, 192));
	text.setPosition(sf::Vector2f(left_top_corner.x + 10, left_top_corner.y + 74 - text.getGlobalBounds().height / 2));
	if (this->isDead())
		text.setFillColor(sf::Color(127, 127, 127));
	win.draw(text);

	//Gotówka
	text_str.clear();
	text_str = " x ";
	ss << this->cash;
	number = ss.str();
	ss.str("");

	text_str += number;
	text.setString(text_str);
	text.setPosition(sf::Vector2f(left_top_corner.x + 20, left_top_corner.y + 52 - text.getGlobalBounds().height / 2));
	win.draw(text);

	//¯ycia
	if (this->life <= 6)
	{
		for (short i = 0; i < this->life; i++)
		{
			this->heart.setPosition(left_top_corner.x + 10 + i * 16, left_top_corner.y + 30);
			win.draw(this->heart);
		}
	}
	else
	{
		this->heart.setPosition(left_top_corner.x + 10, left_top_corner.y + 30);
		win.draw(this->heart);

		text_str.clear();
		text_str = L" x ";
		ss << this->life;
		number = ss.str();
		ss.str("");

		text_str += number;
		text.setString(text_str);
		text.setPosition(left_top_corner.x + 10 + this->heart.getTextureRect().width, left_top_corner.y + 30 - text.getGlobalBounds().height / 2);
		win.draw(text);
	}

	//Poziom postaci
	text_str.clear();
	text_str = "";
	ss << this->lvl;
	number = ss.str();
	ss.str("");

	text_str += number;
	text.setString(text_str);
	text.setFillColor(sf::Color(0, 128, 0, 192));
	text.setCharacterSize(8);
	text.setPosition(sf::Vector2f(left_top_corner.x + 8, left_top_corner.y + 95 - text.getGlobalBounds().height / 2));
	win.draw(text);

	//Doœwiadczenie
	this->exp_bar.setPosition(sf::Vector2f(left_top_corner.x + 26, left_top_corner.y + 94));
	this->exp_bar.setTextureRect(sf::IntRect(0, 0, this->exp * t_experience_bar.getSize().x / this->requiredExpToLevelUp(), this->exp_bar.getTexture()->getSize().y));
	win.draw(exp_bar);

	//Bonusy (pod statystykami)
	text.setFillColor(sf::Color(255, 255, 255, 192));
	text.setCharacterSize(20);

	for (short i = 0; i < 2; i++)
	{
		this->bonus_sprite[i].setPosition(left_top_corner.x + i * 64, left_top_corner.y + t_stats_window.getSize().y + 4);
		win.draw(this->bonus_sprite[i]);

		text_str.clear();
		text_str = L" x ";
		ss << this->bonus[i];
		number = ss.str();
		ss.str("");

		text_str += number;
		text.setCharacterSize(18);
		text.setString(text_str);
		text.setPosition(left_top_corner.x + this->bonus_sprite[i].getTextureRect().width + i * 64, left_top_corner.y + t_stats_window.getSize().y + 6 - text.getGlobalBounds().height / 2);
		win.draw(text);
	}
}

void cCharacter::drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed)
{
	;
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

sControlKeys cCharacter::getControlKeys()
{
	return this->key;
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

bool cCharacter::isSpecial1()
{
	if (this->special1_time > 0)
		return true;
	return false;
}

unsigned int cCharacter::getCash()
{
	return this->cash;
}

unsigned short cCharacter::getLevel()
{
	return this->lvl;
}

unsigned short cCharacter::getSkillPoints()
{
	return this->skill_points;
}

unsigned int cCharacter::requiredExpToLevelUp()
{
	return pow(75.0, 1.0 + (this->lvl - 1) * 0.05);
}