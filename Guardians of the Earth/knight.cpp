#include "knight.h"

cKnight::cKnight(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators)
	:cCharacter(physics_world, world_type, pos, player_no, modulators)
{
	this->setTexture(t_character[0]);
	this->can_crush = true;
}

void cKnight::control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet)
{
	if (this->isSpecial1())
		this->animationSpecial1();

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
			if (this->body->GetLinearVelocity().x > max_speed_x * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2(max_speed_x * speed_multipler, this->body->GetLinearVelocity().y));
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
			if (this->body->GetLinearVelocity().x < -max_speed_x * speed_multipler)
				this->body->SetLinearVelocity(b2Vec2(-max_speed_x * speed_multipler, this->body->GetLinearVelocity().y));
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
					this->jump(-4.0f);
				else
					this->jump(-5.0f);
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
					if (this->isSpecial1())
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