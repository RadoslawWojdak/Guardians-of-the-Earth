#include "spy.h"

cSpy::cSpy(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacter(physics_world, world_type, pos, player_no, modulators)
{
	this->character_type = CHARACTER_SPY;
	this->setTexture(t_character[this->character_type]);

	this->can_crush = false;

	this->bonus[0] = 8;
	this->bonus[1] = 0;

	this->ivisibility_timer = 0;
	this->taser_timer = 0;

	this->has_taser = true;

	for (short i = 0; i < 2; i++)
	{
		this->bonus_sprite[i].setTexture(t_characters_bonus_icon[this->character_type][i]);
		this->bonus_sprite[i].setColor(sf::Color(this->bonus_sprite[i].getColor().g, this->bonus_sprite[i].getColor().b, this->bonus_sprite[i].getColor().a, 192));
	}
}

void cSpy::addPower(short power_id)
{
	switch (power_id)
	{
	case 1: {this->bonus[0] += 8; break;}
	case 2: {this->bonus[1]++; break;}
	}
}

void cSpy::startShooting()
{
	this->animationSpecial1(3);
}

bool cSpy::isShooting()
{
	if (this->getAnimationType() == ANIMATION_SPECIAL1)
		return true;
	return false;
}

void cSpy::startInvisibility()
{
	this->ivisibility_timer = 600 + 60 * this->number_of_skill[2];
	this->immunity_time = 600 + 60 * this->number_of_skill[2];
	
	for (int i = 0; i < this->number_of_skill[3]; i++)
		this->extra_speed += 0.343f * (pow(0.8f, 0.2f * i));	//(max ~ 7.5 (pierwszy skill ~ 0.33 - ka¿dy nastêpny ma coraz mniejszy krok wzrostu))
}

void cSpy::invisibilityCountdown()
{
	if (this->isInvisibility())
	{
		this->ivisibility_timer--;

		if (this->ivisibility_timer == 0)
			this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 255));
		else if (this->ivisibility_timer < 75)
			this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a + 1));
		else if (this->getColor().a == 255)
			this->stopInvisibility();
	}
}

void cSpy::stopInvisibility()
{
	this->ivisibility_timer = 0;
	this->extra_speed = 0;

	this->setColor(sf::Color(this->getColor().r, this->getColor().g, this->getColor().b, 127));
}

bool cSpy::isInvisibility()
{
	if (this->ivisibility_timer > 0)
		return true;
	return false;
}

void cSpy::usedTaser()
{
	this->taser_timer = 900;
	this->has_taser = false;
}

void cSpy::taserCountdown()
{
	if (this->taser_timer > 0)
	{
		this->taser_timer--;
		if (this->taser_timer == 0)
			this->has_taser = true;
	}
}

bool cSpy::canUseTaser()
{
	if (this->taser_timer == 0)
		return true;
	return false;
}

void cSpy::control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet)
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
		{
			this->possible_extra_jumps = this->extra_jump;
			this->can_jump = true;
		}

		if (!this->is_on_ladder && !this->isShooting())
		{
			if (!this->can_jump && this->is_immersed_in == FLUID_NONE)
				this->animationJumping();
			else if (this->body->GetLinearVelocity().y > 0.0f && this->is_immersed_in == FLUID_NONE)
				this->animationStanding();
			else if (this->body->GetLinearVelocity().y != 0.0f && this->is_immersed_in != FLUID_NONE)
				this->animationSwimming();
		}

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.right.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) > 1.0f))
		{
			this->dir = DIR_RIGHT;

			if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isShooting())
				this->animationWalking();

			this->setScale(1.0f, 1.0f);

			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x > (max_speed_x + this->extra_speed) * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2((max_speed_x + this->extra_speed) * speed_multipler, this->body->GetLinearVelocity().y));
			else if (this->body->GetLinearVelocity().x < 0)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
		}

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.left.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) < -1.0f))
		{
			this->dir = DIR_LEFT;

			if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isShooting())
				this->animationWalking();

			this->setScale(-1.0f, 1.0f);

			this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			if (this->body->GetLinearVelocity().x < -(max_speed_x + this->extra_speed) * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2(-(max_speed_x + this->extra_speed) * speed_multipler, this->body->GetLinearVelocity().y));
			else if (this->body->GetLinearVelocity().x > 0)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
		}

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.up.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.up.button)))
			this->dir = DIR_UP;
		else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.down.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.down.button)))
			this->dir = DIR_DOWN;

		//Gdy nie jest naciœniêty ¿aden z klawiszy (lewo, prawo), to postaæ zaczyna siê zatrzymywaæ
		if ((!this->key.is_pad && (!sf::Keyboard::isKeyPressed(this->key.right.key) && !sf::Keyboard::isKeyPressed(this->key.left.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) < 1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) > -1.0f)))
		{
			if (this->can_jump && !this->is_on_ladder && !this->isShooting())
				this->animationStanding();

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
		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button)))
		{
			if (!stop_jump)
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, this->body->GetLinearVelocity().y * 1.022f));
			if ((this->body->GetLinearVelocity().y == 0 && this->can_jump) || (this->body->GetLinearVelocity().y > 0 && this->body->GetLinearVelocity().y < 3.0f && this->possible_extra_jumps > 0) || (this->body->GetLinearVelocity().y >= 0 && this->is_immersed_in != FLUID_NONE))	//1 - W przypadku gdy spadnie sk¹dœ (can_jump jest aktywne); 2 - W przypadku, gdy akurat prêdkoœæ Y by³aby równa 0 (miêdzy wyskokiem a upadkiem); 3 - W przypadku gdy obiekt jest zanurzony w p³ynie
			{
				if (this->body->GetLinearVelocity().y > 0 && this->body->GetLinearVelocity().y < 3.0f && this->possible_extra_jumps > 0)	//Dodatkowe skoki s¹ ni¿sze ni¿ g³ówne
					this->jump(-4.0f - ((float)this->extra_height_of_jump / 1.5f));
				else
					this->jump(-5.0f - this->extra_height_of_jump);
				if (this->is_immersed_in == FLUID_NONE)
					this->stop_jump = false;
			}
		}
		else
			this->stop_jump = true;

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.fire.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.fire.button)))
		{
			if (!this->fire)
			{
				this->fire = true;
				if (this->bonus[0] > 0)
				{
					this->startShooting();
				}
			}
		}
		else
			this->fire = false;

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.special1.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.special1.button)))
		{
			if (this->bonus[1] > 0 && !this->isInvisibility())
			{
				bonus[1]--;
				this->startInvisibility();
			}
		}
	}
}

void cSpy::specialCollisions(b2World *physics_world, eWorld world_type, bool *modulators, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block)
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
						this->jump(-5.0f - this->extra_height_of_jump);
						this->stop_jump = false;
					}
					else
						this->jump(-3.0f - this->extra_height_of_jump / 1.35f);

					this->possible_extra_jumps = this->extra_jump;
				}
				else	//Je¿eli dotkniêto NPC-a w inny sposób
				{
					if (!this->canUseTaser())
					{
						if (!this->isInviolability())
							this->beenHit();
					}
					else
					{
						this->usedTaser();
						npc[i].kill();
						this->addStatsForNPC(npc[i]);
						npc.erase(npc.begin() + i);
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
					this->jump(-6.0f - this->extra_height_of_jump);
					this->stop_jump = false;
				}
				else
					this->jump(-4.0f - this->extra_height_of_jump / 1.5f);

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
						this->body->SetLinearVelocity(b2Vec2(0.0f, -(this->max_speed_x + this->extra_speed) * 0.39f));
						if (!this->isSpecial1())
							this->animationClimbing(true);
					}
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.down.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f))
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

					this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));	//Dziêki temu postaæ bêdzie siê natychmiastowo zatrzymywaæ, gdy gracz póœci klawisz w bok (lewo lub prawo)

					if ((!this->key.is_pad && (!sf::Keyboard::isKeyPressed(this->key.up.key) && !sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > -1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));

					if ((!this->key.is_pad && (sf::Keyboard::isKeyPressed(this->key.up.key) && sf::Keyboard::isKeyPressed(this->key.down.key))) || (this->key.is_pad && (sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) < -1.0f && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::Y) > 1.0f)))
						this->body->SetLinearVelocity(b2Vec2(0.0f, this->body->GetLinearVelocity().y));
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.left.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) < -1.0f))
					{
						this->dir = DIR_LEFT;
						this->body->SetLinearVelocity(b2Vec2(-(this->max_speed_x + this->extra_speed) * 0.222f, this->body->GetLinearVelocity().y));
					}
					else if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.right.key)) || (this->key.is_pad && sf::Joystick::getAxisPosition(this->key.pad, sf::Joystick::X) > 1.0f))
					{
						this->dir = DIR_RIGHT;
						this->body->SetLinearVelocity(b2Vec2((this->max_speed_x + this->extra_speed) * 0.222f, this->body->GetLinearVelocity().y));
					}

					if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.jump.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.jump.button)))
					{
						this->jump(-5.0f - this->extra_height_of_jump);
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

void cSpy::shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction)
{
	if (this->bonus[0] > 0)
	{
		this->bonus[0]--;

		if (shot_direction == DIR_UP)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[2][0], false, b2Vec2(0.0f, -15.0f), sf::Vector2f(this->getPosition().x, this->getPosition().y - this->getOrigin().y + 4), 1.0f + this->number_of_skill[0] * 0.5f, 1, this->number_of_skill[1], this->player_no));
		else if (shot_direction == DIR_DOWN)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[2][0], false, b2Vec2(0.0f, 15.0f), sf::Vector2f(this->getPosition().x, this->getPosition().y + this->getOrigin().y - 4), 1.0f + this->number_of_skill[0] * 0.5f, 1, this->number_of_skill[1], this->player_no));
		else if (shot_direction == DIR_LEFT)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[2][0], false, b2Vec2(-15.0f, 0.0f), sf::Vector2f(this->getPosition().x - this->getOrigin().x + 4, this->getPosition().y), 1.0f + this->number_of_skill[0] * 0.5f, 1, this->number_of_skill[1], this->player_no));
		else if (shot_direction == DIR_RIGHT)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[2][0], false, b2Vec2(15.0f, 0.0f), sf::Vector2f(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y), 1.0f + this->number_of_skill[0] * 0.5f, 1, this->number_of_skill[1], this->player_no));
	}
}

void cSpy::checkIndicators(b2World *world, eWorld world_type, std::vector <cCharacter*> player, std::vector <cBullet> &bullet)
{
	this->immunityCountdown();
	//Timer u¿ywania bonusu 1
	if (this->isShooting())
	{
		this->animationSpecial1(3);
		if (this->isAnimationBeginsAgain())
		{
			this->animationStanding();
			if (this->is_immersed_in == FLUID_NONE)
				this->shot(world, world_type, bullet, this->dir);
		}
	}
	//!Timer u¿ywania bonusu 1
	this->invisibilityCountdown();
	this->taserCountdown();
	
	if (this->exp >= this->requiredExpToLevelUp())
		this->levelUp();
}

void cSpy::addSkill(unsigned short skill_id)
{
	this->number_of_skill[skill_id - 1]++;
	this->skill_points--;
}

void cSpy::drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed)
{
	//Napisy dotycz¹ce postaci gracza
	//Poziom postaci
	sf::String text_str;
	std::string number;
	std::stringstream ss;
	ss << this->lvl;
	number = ss.str();
	ss.str("");

	text_str = number;
	sf::Text text(text_str, font[1], 32);
	text.setFillColor(sf::Color(48, 128, 0));
	text.setPosition(sf::Vector2f(left_top_corner.x, left_top_corner.y - text.getGlobalBounds().height / 2));
	win.draw(text);

	//Punkty umiejêtnoœci
	text_str.clear();
	ss << this->skill_points;
	number = ss.str();
	ss.str("");

	text_str = number;
	text.setString(text_str);
	text.setFillColor(sf::Color(255, 215, 0));
	text.setPosition(sf::Vector2f(left_top_corner.x + 96, left_top_corner.y - text.getGlobalBounds().height / 2));
	win.draw(text);

	//Obrazki nad umiejêtnoœciami
	this->bonus_sprite[0].setPosition(sf::Vector2f(left_top_corner.x + 32 + t_characters_skill[this->character_type][0].getSize().x / 2 - this->bonus_sprite[0].getTextureRect().width / 2, left_top_corner.y + 100));
	win.draw(this->bonus_sprite[0]);

	this->bonus_sprite[1].setPosition(sf::Vector2f(left_top_corner.x + 80 + t_characters_skill[this->character_type][2].getSize().x / 2 - this->bonus_sprite[1].getTextureRect().width / 2, left_top_corner.y + 100));
	win.draw(this->bonus_sprite[1]);

	//Umiejêtnoœci
	text.setCharacterSize(12);

	for (unsigned short i = 0; i < 4; i++)
	{
		sf::Vector2f pos;
		unsigned short required_level;

		switch (i)
		{
		case 0: {pos.x = left_top_corner.x + 32; pos.y = left_top_corner.y + 132; required_level = 1; break;}
		case 1: {pos.x = left_top_corner.x + 32; pos.y = left_top_corner.y + 180; required_level = 5; break;}
		case 2: {pos.x = left_top_corner.x + 80; pos.y = left_top_corner.y + 132; required_level = 1; break;}
		case 3: {pos.x = left_top_corner.x + 80; pos.y = left_top_corner.y + 228; required_level = 10; break;}
		}

		//Umiejêtnoœæ
		s_characters_skill[this->character_type][i].setPosition(pos);
		if (selected_skill == i + 1)
			s_characters_skill[this->character_type][i].setColor(sf::Color(255, 255, 255));
		else if (this->lvl >= required_level)
			s_characters_skill[this->character_type][i].setColor(sf::Color(128, 128, 128));
		else
			s_characters_skill[this->character_type][i].setColor(sf::Color(16, 16, 16));
		win.draw(s_characters_skill[this->character_type][i]);

		//Napis
		if (this->lvl >= required_level)
		{
			ss << this->number_of_skill[i];
			number = ss.str();
			ss.str("");

			text.setString(number);
			text.setFillColor(sf::Color(255, 215, 0));
			text.setPosition(sf::Vector2f(pos.x + s_characters_skill[this->character_type][i].getTextureRect().width - 4, pos.y + s_characters_skill[this->character_type][i].getTextureRect().height - 4));
			win.draw(text);
		}
	}

	//Przycisk wyjœcia z Menu Drzewka Umiejêtnoœci
	cButton exit_button(sf::Vector2f(left_top_corner.x + 56, left_top_corner.y + 320), "Ready");
	exit_button.changeGraphics(selected_skill == 0, (close_pressed ? sf::Color(64, 255, 64) : sf::Color(255, 192, 0)));
	exit_button.draw(win);

	//Wymagane poziomy
	text.setFont(font[1]);
	text.setCharacterSize(20);
	if (this->lvl >= 5)
		text.setFillColor(sf::Color(255, 255, 255));
	else
		text.setFillColor(sf::Color(32, 32, 32));
	text.setString("5");
	text.setPosition(sf::Vector2f(left_top_corner.x, left_top_corner.y + 190 - text.getGlobalBounds().height / 2));
	win.draw(text);

	text.setString("10");
	if (this->lvl >= 10)
		text.setFillColor(sf::Color(255, 255, 255));
	else
		text.setFillColor(sf::Color(32, 32, 32));
	text.setPosition(sf::Vector2f(left_top_corner.x, left_top_corner.y + 238 - text.getGlobalBounds().height / 2));
	win.draw(text);
}