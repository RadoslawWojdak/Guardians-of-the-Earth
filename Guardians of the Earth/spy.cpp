#include "spy.h"

cSpy::cSpy(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacter(physics_world, world_type, pos, player_no, modulators)
{
	this->character_type = CHARACTER_SPY;
	this->setTexture(t_character[2]);

	this->can_crush = false;

	this->bonus[0] = 8;
	this->bonus[1] = 0;

	this->ivisibility_timer = 0;

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
		this->extra_speed += 0.343f * (pow(0.8f, 0.2f * i));	//(max ~ 7.5 (pierwszy skill ~ 0.33 - ka�dy nast�pny ma coraz mniejszy krok wzrostu))
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

		//Gdy nie jest naci�ni�ty �aden z klawiszy (lewo, prawo), to posta� zaczyna si� zatrzymywa�
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
			if ((this->body->GetLinearVelocity().y == 0 && this->can_jump) || (this->body->GetLinearVelocity().y > 0 && this->body->GetLinearVelocity().y < 3.0f && this->possible_extra_jumps > 0) || (this->body->GetLinearVelocity().y >= 0 && this->is_immersed_in != FLUID_NONE))	//1 - W przypadku gdy spadnie sk�d� (can_jump jest aktywne); 2 - W przypadku, gdy akurat pr�dko�� Y by�aby r�wna 0 (mi�dzy wyskokiem a upadkiem); 3 - W przypadku gdy obiekt jest zanurzony w p�ynie
			{
				if (this->body->GetLinearVelocity().y > 0 && this->body->GetLinearVelocity().y < 3.0f && this->possible_extra_jumps > 0)	//Dodatkowe skoki s� ni�sze ni� g��wne
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

void cSpy::checkIndicators(b2World *world, eWorld world_type, std::vector <cBullet> &bullet)
{
	this->immunityCountdown();
	//Timer u�ywania bonusu 1
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
	//!Timer u�ywania bonusu 1
	this->invisibilityCountdown();
	
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
	//Napisy dotycz�ce postaci gracza
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

	//Punkty umiej�tno�ci
	text_str.clear();
	ss << this->skill_points;
	number = ss.str();
	ss.str("");

	text_str = number;
	text.setString(text_str);
	text.setFillColor(sf::Color(255, 215, 0));
	text.setPosition(sf::Vector2f(left_top_corner.x + 96, left_top_corner.y - text.getGlobalBounds().height / 2));
	win.draw(text);

	//Obrazki nad umiej�tno�ciami
	this->bonus_sprite[0].setPosition(sf::Vector2f(left_top_corner.x + 32 + t_characters_skill[this->character_type][0].getSize().x / 2 - this->bonus_sprite[0].getTextureRect().width / 2, left_top_corner.y + 100));
	win.draw(this->bonus_sprite[0]);

	this->bonus_sprite[1].setPosition(sf::Vector2f(left_top_corner.x + 80 + t_characters_skill[this->character_type][2].getSize().x / 2 - this->bonus_sprite[1].getTextureRect().width / 2, left_top_corner.y + 100));
	win.draw(this->bonus_sprite[1]);

	//Umiej�tno�ci
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

		//Umiej�tno��
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

	//Przycisk wyj�cia z Menu Drzewka Umiej�tno�ci
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