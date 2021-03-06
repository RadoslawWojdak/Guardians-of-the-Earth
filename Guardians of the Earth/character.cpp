#include "character.h"

cCharacter::cCharacter(b2World &physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
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
	this->taser.setTexture(t_taser);
	this->taser.setColor(sf::Color(this->taser.getColor().g, this->taser.getColor().b, this->taser.getColor().a, 192));
	this->magic_shield.setTexture(t_magic_shield);
	this->magic_shield.setColor(sf::Color(this->magic_shield.getColor().g, this->magic_shield.getColor().b, this->magic_shield.getColor().a, 0));
	this->magic_shield.setOrigin(this->magic_shield.getTextureRect().width / 2, this->magic_shield.getTextureRect().height / 2);

	this->immunity_time = 0;
	this->magic_shield_time = 0;
	this->special1_time = 0;
	this->life = 3;
	this->score = 0;
	this->cash = 0;
	this->has_taser = false;

	this->max_speed_x = 4.5f;
	this->extra_speed = 0.0f;
	this->extra_jump = 0;
	this->extra_height_of_jump = 0.0f;
	if (modulators[1])
		this->extra_jump++;
	this->last_position = this->getPosition();

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

	this->body = physics_world.CreateBody(&body_def);
	
	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.06f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_CHARACTER);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : CATEGORY(CAT_EMPTY));

	this->body->CreateFixture(&fd);
}

void cCharacter::loadParameters(unsigned short lvl, unsigned int exp, unsigned short skill_point, unsigned short number_of_skill[4], unsigned short pet_hp, unsigned int bonus[2], unsigned short life, unsigned int score, unsigned int cash, bool has_taser)
{
	this->lvl = lvl;
	this->exp = exp;
	this->skill_points = skill_point;

	for (int i = 0; i < 4; i++)
		this->number_of_skill[i] = number_of_skill[i];

	pet.kill();
	for (int i = 0; i < pet_hp; i++)
		this->addHP();

	for (int i = 0; i < 2; i++)
		this->bonus[i] = bonus[i];

	this->life = life;
	this->score = score;
	this->cash = cash;
	this->has_taser = has_taser;
}

void cCharacter::loadCharacter(unsigned short lvl, unsigned int exp, unsigned short skill_point, unsigned short number_of_skill[4], unsigned short pet_hp, unsigned int bonus[2], unsigned short life, unsigned int score, unsigned int cash, bool has_taser)
{
	;
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
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : CATEGORY(CAT_EMPTY));

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

void cCharacter::useMagicShield(unsigned int time)
{
	this->magic_shield_time = time;
}

void cCharacter::magicShieldCountdown()
{
	if (this->hasMagicShield())
	{
		this->magic_shield_time--;

		if (this->magic_shield_time == 0)
			this->turnOffMagicShield();
		else if (this->magic_shield_time < 240)
			this->magic_shield.setColor(sf::Color(this->magic_shield.getColor().r, this->magic_shield.getColor().g, this->magic_shield.getColor().b, this->magic_shield_time / 2 + 60));
		else if (this->magic_shield.getColor().a == 0)
			this->magic_shield.setColor(sf::Color(this->magic_shield.getColor().r, this->magic_shield.getColor().g, this->magic_shield.getColor().b, 190));
	}
}

void cCharacter::turnOffMagicShield()
{
	this->magic_shield_time = 0;
	this->magic_shield.setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 0));
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

void cCharacter::control(b2World &physics_world, eWorld world_type, std::vector <cBullet> &bullet)
{
	;	//Funkcja wirtualna
}

void cCharacter::specialCollisions(b2World &physics_world, eWorld world_type, bool *modulators, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block)
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
				if ((this->last_position.y + this->getOrigin().y <= npc[i].getLastPosition().y - this->getOrigin().y + 3 && (!npc[i].getFeatures().top_hurts || this->isSpecial1())))	//Je�eli posta� spad�a na NPC-a; last_position naprawia b��dy zwi�zane z �mierci� postaci, gdy spada�a zbyt szybko; +3 - gdy posta� znajduje si� tu� nad NPC-em i chce na niego spa�� (gracz nie chodi tu� nad pod�o�em, lecz bezpo�rednio na nim)
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

					if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].jump.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].jump.button)))
					{
						this->jump(-5.0f - this->extra_height_of_jump);
						this->stop_jump = false;
					}
					else
						this->jump(-3.0f - this->extra_height_of_jump / 1.35f);

					this->possible_extra_jumps = this->extra_jump;
				}
				else	//Je�eli dotkni�to NPC-a w inny spos�b
				{
					if (!this->isSpecial1())
					{
						if (this->hasMagicShield())
						{
							this->turnOffMagicShield();
							
							npc[i].kill();
							this->addStatsForNPC(npc[i]);
							npc.erase(npc.begin() + i);
						}
						else if (!this->isInviolability())
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
			if (this->can_jump)	//Dopiero jak stanie na czym� innym ni� l�d, to przestaje si� �lizga�
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
				if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].jump.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].jump.button)))
				{
					this->jump(-6.0f - this->extra_height_of_jump);
					this->stop_jump = false;
				}
				else
					this->jump(-4.0f - this->extra_height_of_jump / 1.5f);

				this->possible_extra_jumps = this->extra_jump;
				break;	//Nawet gdyby by�o wi�cej trampolin to nie robi�oby to r�nicy
			}
		}

		//Kolizje z drabinami
		bool ladder_collision = false;
		for (unsigned int i = 0; i < ladder.size(); i++)
		{
			if (this->getGlobalBounds().intersects(ladder[i].getGlobalBounds()))
			{
				ladder_collision = true;
				
				if ((!g_key[this->player_no - 1].is_pad && (sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key) || sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key))) || (g_key[this->player_no - 1].is_pad && (sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) < -1.0f || sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) > 1.0f)))
				{
					this->dir = DIR_UP;

					this->body->SetGravityScale(0.0f);
					this->is_on_ladder = true;
					this->stop_jump = true;
					this->can_jump = true;

					if ((!g_key[this->player_no - 1].is_pad && (sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key) && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key))) || (g_key[this->player_no - 1].is_pad && (sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) < -1.0f && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) > 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					else if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) < -1.0f))
					{
						this->dir = DIR_UP;
						this->body->SetLinearVelocity(b2Vec2(0.0f, -(this->max_speed_x + this->extra_speed) * 0.39f));
						if (!this->isSpecial1())
							this->animationClimbing(true);
					}
					else if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) > 1.0f))
					{
						this->dir = DIR_DOWN;
						this->body->SetLinearVelocity(b2Vec2(0.0f, (this->max_speed_x + this->extra_speed) * 0.39f));
						if (!this->isSpecial1())
							this->animationClimbing(false);
					}
				}
				
				if (this->is_on_ladder)
				{
					this->possible_extra_jumps = this->extra_jump;

					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));	//Dzi�ki temu posta� b�dzie si� natychmiastowo zatrzymywa�, gdy gracz p�ci klawisz w bok (lewo lub prawo)

					if ((!g_key[this->player_no - 1].is_pad && (!sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key) && !sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key))) || (g_key[this->player_no - 1].is_pad && (sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) > -1.0f && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) < 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));

					if ((!g_key[this->player_no - 1].is_pad && (sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key) && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key))) || (g_key[this->player_no - 1].is_pad && (sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) < -1.0f && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::Y) > 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
					else if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].left.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) < -1.0f))
					{
						this->dir = DIR_LEFT;
						this->body->SetLinearVelocity(b2Vec2(-(this->max_speed_x + this->extra_speed) * 0.222f, this->body->GetLinearVelocity().y));
					}
					else if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].right.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) > 1.0f))
					{
						this->dir = DIR_RIGHT;
						this->body->SetLinearVelocity(b2Vec2((this->max_speed_x + this->extra_speed) * 0.222f, this->body->GetLinearVelocity().y));
					}

					if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].jump.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].jump.button)))
					{
						this->jump(-5.0f - this->extra_height_of_jump);
						this->stop_jump = false;
						this->is_on_ladder = false;
					}
				}

				break;	//Nawet gdyby by�o wi�cej drabin to nie robi�oby to r�nicy
			}
		}
		if (!ladder_collision)
			this->is_on_ladder = false;
	}
}

void cCharacter::applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size)
{
	if (!this->isDead() && !this->is_on_ladder)	//Gdy gracz jest na drabinie, nie dzia�a na niego �adna grawitacja
	{
		//Gracz nie mo�e przyspieszy� skoku (zwi�ksza� wysoko�ci skoku) postaci, gdy ta spada
		if (this->body->GetLinearVelocity().y >= 0)
			this->stop_jump = true;

		//Kolizje z p�ynami (zmiana grawitacji, oraz pr�dko�ci)
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
				else if (this->body->GetLinearVelocity().y < -0.5f - this->extra_height_of_jump * 0.1f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -0.5f - this->extra_height_of_jump * 0.1f));

				break;
			}
			default:
			{
				this->is_immersed_in = FLUID_WATER;

				this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 2.0f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 2.0f));
				else if (this->body->GetLinearVelocity().y < -2.0f - this->extra_height_of_jump * 0.4f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -2.0f - this->extra_height_of_jump * 0.4f));

				break;
			}
			}

			if (world_type != WORLD_ICE_LAND)
				this->possible_extra_jumps = this->extra_jump;
		}
		else
		{
			if (this->is_immersed_in != FLUID_NONE && ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].jump.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].jump.button))))	//Je�eli wyskoczy� z wody
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, -5.0f - this->extra_height_of_jump));

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
		
		//Czy gracz wylecia� poza ramk� poziomu
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

void cCharacter::checkIndicators(b2World &world, eWorld world_type, std::vector <cCharacter*> player, std::vector <cBullet> &bullet)
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
	this->magic_shield_time = 0;
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
	case 1: {this->addPower(1); this->score += 100 * g_score_multipler; break;}	//+10 pocisk�w
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
}

void cCharacter::subtractCash(unsigned int how_many_to_subtract)
{
	this->cash -= how_many_to_subtract;
}

void cCharacter::draw(sf::RenderWindow &win)
{
	win.draw(*this);

	this->magic_shield.setPosition(this->getPosition());
	win.draw(this->magic_shield);
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

	//Got�wka
	text_str.clear();
	text_str = " x ";
	ss << this->cash;
	number = ss.str();
	ss.str("");

	text_str += number;
	text.setString(text_str);
	text.setPosition(sf::Vector2f(left_top_corner.x + 20, left_top_corner.y + 52 - text.getGlobalBounds().height / 2));
	win.draw(text);

	//�ycia
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

	//Do�wiadczenie
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

	//Paralizator
	if (this->has_taser)
	{
		this->taser.setPosition(left_top_corner.x, left_top_corner.y + t_stats_window.getSize().y + this->bonus_sprite[0].getTextureRect().height + 12);
		win.draw(this->taser);
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

short cCharacter::getPlayerNo()
{
	return this->player_no;
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

bool cCharacter::hasMagicShield()
{
	if (this->magic_shield_time > 0)
		return true;
	return false;
}

bool cCharacter::isSpecial1()
{
	if (this->special1_time > 0)
		return true;
	return false;
}

eCharacter cCharacter::getCharacterType()
{
	return this->character_type;
}

unsigned short cCharacter::getLife()
{
	return this->life;
}

unsigned short cCharacter::getHP()
{
	return this->pet.getHP();
}

unsigned int cCharacter::getScore()
{
	return this->score;
}

unsigned int cCharacter::getCash()
{
	return this->cash;
}

unsigned int cCharacter::getBonus(short bonus_id)
{
	return this->bonus[bonus_id];
}

unsigned short cCharacter::getLevel()
{
	return this->lvl;
}

unsigned int cCharacter::getExperience()
{
	return this->exp;
}

unsigned short cCharacter::getSkillPoints()
{
	return this->skill_points;
}

unsigned short cCharacter::getNumberOfSkill(short skill_id)
{
	return this->number_of_skill[skill_id];
}

unsigned int cCharacter::requiredExpToLevelUp()
{
	return pow(75.0, 1.0 + (this->lvl - 1) * 0.05);
}

bool cCharacter::hasTaser()
{
	return this->has_taser;
}