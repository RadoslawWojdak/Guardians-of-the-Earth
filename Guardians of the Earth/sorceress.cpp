#include "sorceress.h"

cSorceress::cSorceress(b2World &physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacter(physics_world, world_type, pos, player_no, modulators)
{
	this->character_type = CHARACTER_SORCERESS;
	this->setTexture(t_character[this->character_type]);

	this->can_crush = false;

	this->bonus[0] = 3;
	this->bonus[1] = 1;

	for (short i = 0; i < 2; i++)
	{
		this->bonus_sprite[i].setTexture(t_characters_bonus_icon[this->character_type][i]);
		this->bonus_sprite[i].setColor(sf::Color(this->bonus_sprite[i].getColor().g, this->bonus_sprite[i].getColor().b, this->bonus_sprite[i].getColor().a, 192));
	}
}

void cSorceress::startShooting()
{
	this->animationSpecial1(6);
}

bool cSorceress::isShooting()
{
	if (this->getAnimationType() == ANIMATION_SPECIAL1)
		return true;
	return false;
}

void cSorceress::startHealing()
{
	this->animationSpecial2(10);
}

bool cSorceress::isHealing()
{
	if (this->getAnimationType() == ANIMATION_SPECIAL2)
		return true;
	return false;
}

void cSorceress::heal(b2World &physics_world, eWorld world_type, std::vector <cCharacter*> player)
{
	if (this->bonus[1] > 0)
	{
		this->bonus[1]--;

		int chance_for_rebirth = this->number_of_skill[3] * 5;
		int random = rand() % 100 + 1;

		bool is_dead[4] = {};

		for (int i = 0; i < player.size(); i++)
		{
			if (!player[i]->isDead())
			{
				player[i]->addHP();
				player[i]->useMagicShield(this->number_of_skill[2] * 150);
			}
			else
				is_dead[i] = true;
		}

		//Odrodzenie innej postaci, je¿eli wylosowano odrodzenie
		if (random <= chance_for_rebirth && (is_dead[0] || is_dead[1] || is_dead[2] || is_dead[3]))
		{
			for (;;)
			{
				short rebirth = rand() % 4;
				if (is_dead[rebirth])
				{
					player[rebirth]->bodyRecreate(physics_world, world_type);
					player[rebirth]->rebirth();
					player[rebirth]->setAllPositions(this->getPosition());

					break;
				}
			}
		}
	}
}

void cSorceress::addPower(short power_id)
{
	switch (power_id)
	{
	case 1: {this->bonus[0] += 3; break;}
	case 2: {this->bonus[1]++; break;}
	}
}


void cSorceress::control(b2World &physics_world, eWorld world_type, std::vector <cBullet> &bullet)
{
	if (!this->isDead())
	{
		float speed_multipler = 1;
		switch (this->is_immersed_in)
		{
		case FLUID_WATER: {speed_multipler = g_fluid_speed_multipler.water; break;}
		case FLUID_QUICKSAND: {speed_multipler = g_fluid_speed_multipler.quicksand; break;}
		}

		if (this->isShooting() || this->isHealing())
		{
			if (this->isShooting())
				animationSpecial1(6);
			else if (this->isHealing())
				animationSpecial2(10);

			//Postaæ zaczyna siê zatrzymywaæ (podczas strza³u nie mo¿e siê poruszaæ)
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
		else	//Podczas strzelania nie mo¿e siê poruszaæ
		{
			if (this->last_speed.y >= 0 && this->body->GetLinearVelocity().y <= 0)
			{
				this->possible_extra_jumps = this->extra_jump;
				this->can_jump = true;
			}

			if (!this->is_on_ladder && !this->isSpecial1())
			{
				if (!this->can_jump && this->is_immersed_in == FLUID_NONE)
					this->animationJumping();
				else if (this->body->GetLinearVelocity().y > 0.0f && this->is_immersed_in == FLUID_NONE)
					this->animationStanding();
				else if (this->body->GetLinearVelocity().y != 0.0f && this->is_immersed_in != FLUID_NONE)
					this->animationSwimming();
			}

			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].right.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) > 1.0f))
			{
				this->dir = DIR_RIGHT;

				if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isSpecial1())
					this->animationWalking();

				this->setScale(1.0f, 1.0f);

				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x > (this->max_speed_x + this->extra_speed) * speed_multipler)
					this->body->SetLinearVelocity(b2Vec2((this->max_speed_x + this->extra_speed) * speed_multipler, this->body->GetLinearVelocity().y));
				else if (this->body->GetLinearVelocity().x < 0)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x + 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			}

			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].left.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) < -1.0f))
			{
				this->dir = DIR_LEFT;

				if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isSpecial1())
					this->animationWalking();

				this->setScale(-1.0f, 1.0f);

				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.1f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
				if (this->body->GetLinearVelocity().x < -(this->max_speed_x + this->extra_speed) * speed_multipler)
					this->body->SetLinearVelocity(b2Vec2(-(this->max_speed_x + this->extra_speed) * speed_multipler, this->body->GetLinearVelocity().y));
				else if (this->body->GetLinearVelocity().x > 0)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x - 0.2f * speed_multipler * (is_on_ice ? 0.2f : 1) * ((!this->can_jump || this->body->GetLinearVelocity().y) && this->is_immersed_in == FLUID_NONE != 0 ? 0.4f : 1), this->body->GetLinearVelocity().y));
			}

			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].up.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].up.button)))
				this->dir = DIR_UP;
			else if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].down.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].down.button)))
				this->dir = DIR_DOWN;

			//Gdy nie jest naciœniêty ¿aden z klawiszy (lewo, prawo), to postaæ zaczyna siê zatrzymywaæ
			if ((!g_key[this->player_no - 1].is_pad && (!sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].right.key) && !sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].left.key))) || (g_key[this->player_no - 1].is_pad && (sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) < 1.0f && sf::Joystick::getAxisPosition(g_key[this->player_no - 1].pad, sf::Joystick::X) > -1.0f)))
			{
				if (this->can_jump && !this->is_on_ladder && !this->isSpecial1())
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
			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].jump.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].jump.button)))
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

			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].fire.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].fire.button)))
			{
				if (!this->fire)
				{
					this->fire = true;
					if (this->bonus[0] > 0)
						this->startShooting();
				}
			}
			else
				this->fire = false;

			if ((!g_key[this->player_no - 1].is_pad && sf::Keyboard::isKeyPressed(g_key[this->player_no - 1].special1.key)) || (g_key[this->player_no - 1].is_pad && sf::Joystick::isButtonPressed(g_key[this->player_no - 1].pad, g_key[this->player_no - 1].special1.button)))
			{
				if (this->bonus[1] > 0)
				{
					this->startHealing();
				}
			}
		}
	}
}

void cSorceress::shot(b2World &world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction)
{
	if (this->bonus[0] > 0)
	{
		this->bonus[0]--;

		int chance_for_extra_bullet = this->number_of_skill[0] * 10;
		int random = rand() % 100 + 1;

		for (int i = 0; i < 1 + (random <= chance_for_extra_bullet ? 1 : 0); i++)
		{
			if (shot_direction == DIR_UP)
				bullet.push_back(cBullet(world, world_type, t_characters_bonus[this->character_type][0], false, b2Vec2(0.0f, -4.5f), sf::Vector2f(this->getPosition().x + i * 6, this->getPosition().y - this->getOrigin().y + 4), 1.0f, 1 + this->number_of_skill[1], 0, true, this->player_no));
			else if (shot_direction == DIR_DOWN)
				bullet.push_back(cBullet(world, world_type, t_characters_bonus[this->character_type][0], false, b2Vec2(0.0f, 4.5f), sf::Vector2f(this->getPosition().x + i * 6, this->getPosition().y + this->getOrigin().y - 4), 1.0f, 1 + this->number_of_skill[1], 0, true, this->player_no));
			else if (shot_direction == DIR_LEFT)
				bullet.push_back(cBullet(world, world_type, t_characters_bonus[this->character_type][0], false, b2Vec2(-4.5f, 0.0f), sf::Vector2f(this->getPosition().x - this->getOrigin().x + 4, this->getPosition().y - i * 6), 1.0f, 1 + this->number_of_skill[1], 0, true, this->player_no));
			else if (shot_direction == DIR_RIGHT)
				bullet.push_back(cBullet(world, world_type, t_characters_bonus[this->character_type][0], false, b2Vec2(4.5f, 0.0f), sf::Vector2f(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y - i * 6), 1.0f, 1 + this->number_of_skill[1], 0, true, this->player_no));
			bullet[bullet.size() - 1].setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
		}
	}
}

void cSorceress::checkIndicators(b2World &world, eWorld world_type, std::vector <cCharacter*> player, std::vector<cBullet>& bullet)
{
	this->immunityCountdown();
	this->magicShieldCountdown();
	//Timer u¿ywania bonusu 1
	if (this->isShooting())
	{
		if (this->isAnimationBeginsAgain())
		{
			this->animationStanding();
			this->shot(world, world_type, bullet, this->dir);
		}
	}
	//!Timer u¿ywania bonusu 1
	//Timer u¿ywania bonusu 2
	if (this->isHealing())
	{
		if (this->isAnimationBeginsAgain())
		{
			this->animationStanding();
			this->heal(world, world_type, player);
		}
	}
	//!Timer u¿ywania bonusu 2
}

void cSorceress::addSkill(unsigned short skill_id)
{
	this->number_of_skill[skill_id - 1]++;
	this->skill_points--;

	switch (skill_id - 1)
	{
	case 1:
	{
		if (this->number_of_skill[0] > 10)
		{
			this->number_of_skill[0]--;
			this->skill_points++;
		}
	}
	case 4:
	{
		if (this->number_of_skill[3] > 20)
		{
			this->number_of_skill[3]--;
			this->skill_points++;
		}
	}
	}
}

void cSorceress::drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed)
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