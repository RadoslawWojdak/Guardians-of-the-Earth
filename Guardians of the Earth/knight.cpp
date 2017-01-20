#include "knight.h"

cKnight::cKnight(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacter(physics_world, world_type, pos, player_no, modulators)
{
	this->character_type = CHARACTER_KNIGHT;
	this->setTexture(t_character[this->character_type]);
	
	this->can_crush = true;

	this->bonus[0] = 5;
	this->bonus[1] = 0;

	for (short i = 0; i < 2; i++)
	{
		this->bonus_sprite[i].setTexture(t_characters_bonus_icon[this->character_type][i]);
		this->bonus_sprite[i].setColor(sf::Color(this->bonus_sprite[i].getColor().g, this->bonus_sprite[i].getColor().b, this->bonus_sprite[i].getColor().a, 192));
	}
}

void cKnight::addPower(short power_id)
{
	switch (power_id)
	{
	case 1: {this->bonus[0] += 5 + this->number_of_skill[0]; break;}
	case 2: {this->bonus[1]++; break;}
	}
}

void cKnight::control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet)
{
	if (this->isSpecial1())
		this->animationSpecial1(3);
	
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

		if (!this->is_on_ladder && !this->isSpecial1())
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

			if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isSpecial1())
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

			if (this->can_jump && !this->is_on_ladder && this->body->GetLinearVelocity().y == 0.0f && !this->isSpecial1())
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
					this->shot(physics_world, world_type, bullet, this->dir);
					//Je¿eli gracz u¿ywa bonusu 2 (krêcenie siê), to zaczyna strzelaæ pociskami
					if (this->isSpecial1() && this->b1_in_b2_timer == 0)
					{
						this->startB1InB2();
						this->shot_dir = this->dir;
					}
				}
			}
		}
		else
			this->fire = false;

		if ((!this->key.is_pad && sf::Keyboard::isKeyPressed(this->key.special1.key)) || (this->key.is_pad && sf::Joystick::isButtonPressed(this->key.pad, this->key.special1.button)))
		{
			if (!this->isSpecial1())
				this->startSpecial1();
		}
	}
}

void cKnight::shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction)
{
	if (this->bonus[0] > 0)
	{
		this->bonus[0]--;

		if (shot_direction == DIR_UP)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[0][0], false, b2Vec2(0.0f, -4.5f), sf::Vector2f(this->getPosition().x, this->getPosition().y - this->getOrigin().y + 4), 1.0f, 1 + this->number_of_skill[1], 0, this->player_no));
		else if (shot_direction == DIR_DOWN)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[0][0], false, b2Vec2(0.0f, 4.5f), sf::Vector2f(this->getPosition().x, this->getPosition().y + this->getOrigin().y - 4), 1.0f, 1 + this->number_of_skill[1], 0, this->player_no));
		else if (shot_direction == DIR_LEFT)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[0][0], false, b2Vec2(-4.5f, 0.0f), sf::Vector2f(this->getPosition().x - this->getOrigin().x + 4, this->getPosition().y), 1.0f, 1 + this->number_of_skill[1], 0, this->player_no));
		else if (shot_direction == DIR_RIGHT)
			bullet.push_back(cBullet(world, world_type, t_characters_bonus[0][0], false, b2Vec2(4.5f, 0.0f), sf::Vector2f(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y), 1.0f, 1 + this->number_of_skill[1], 0, this->player_no));
	}
}

void cKnight::checkIndicators(b2World *world, eWorld world_type, std::vector <cCharacter*> player, std::vector <cBullet> &bullet)
{
	this->immunityCountdown();
	this->special1Countdown();
	this->b1InB2Countdown(world, world_type, bullet);

	if (this->exp >= this->requiredExpToLevelUp())
		this->levelUp();
}

void cKnight::startSpecial1()
{
	if (this->bonus[1] > 0 && !this->isSpecial1())
	{
		bonus[1]--;
		this->special1_time = 600 + this->number_of_skill[2] * 100;
	}
}

void cKnight::special1Countdown()
{
	if (this->isSpecial1())
		this->special1_time--;
}

void cKnight::startB1InB2()
{
	this->b1_in_b2_timer = 30;
	this->bonus1_in_bonus2 = this->number_of_skill[3];
	this->shot_dir = this->dir;
}

void cKnight::b1InB2Countdown(b2World *world, eWorld world_type, std::vector <cBullet> &bullet)
{
	if (this->bonus1_in_bonus2 > 0)
	{
		if (!this->isSpecial1())
		{
			this->bonus1_in_bonus2 = 0;
			b1_in_b2_timer = 0;
			return;
		}

		b1_in_b2_timer--;
		if (this->b1_in_b2_timer == 0)
		{
			if (this->shot_dir == DIR_RIGHT)
				this->shot_dir = DIR_LEFT;
			else
				this->shot_dir = DIR_RIGHT;

			this->bonus[0]++;	//Przywracanie z powrotem bonusu 1
			this->shot(world, world_type, bullet, this->shot_dir);
			this->bonus1_in_bonus2--;
			if (bonus1_in_bonus2 > 0)
				this->b1_in_b2_timer = 30;
		}
	}
}

void cKnight::addSkill(unsigned short skill_id)
{
	this->number_of_skill[skill_id - 1]++;
	this->skill_points--;
}

void cKnight::drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed)
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