#include "bullet.h"

cBullet::cBullet(b2World &physics_world, eWorld world_type, sf::Texture &texture, bool gravity, b2Vec2 speed, sf::Vector2f pos, float strength, unsigned short piercing, unsigned short bouncing, short player_id)
{
	this->adjustGraphicsParameters(texture, pos);

	this->destroyed = false;
	this->stop = false;
	this->gravity = gravity;
	this->last_pos = pos;
	this->speed = speed;
	this->last_speed = speed;
	this->strength = strength;
	this->piercing = piercing;
	this->bouncing = bouncing;
	this->player_id = player_id;
	
	//BOX2D
	float32 a = this->getTextureRect().width * 0.02f;
	float32 b = this->getTextureRect().height * 0.02f;

	body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
	body_def.type = b2_dynamicBody;
	body_def.allowSleep = true;

	this->body = physics_world.CreateBody(&body_def);
	if (!this->gravity)
		this->body->SetGravityScale(0.0f);
	this->body->SetBullet(true);
	
	b2PolygonShape shape;
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.02f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 20.0f;
	fd.friction = 50.0f;
	if (this->bouncing > 0)
		fd.restitution = 1.0f;

	fd.filter.categoryBits = CATEGORY(CAT_BULLET);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	this->body->CreateFixture(&fd);

	this->body->SetLinearVelocity(speed);
}

void cBullet::step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab)
{
	if (!this->stop)
	{
		if ((this->last_speed.x > -0.02f && this->body->GetLinearVelocity().x < 0.02f && this->last_speed.x != this->body->GetLinearVelocity().x) || (this->last_speed.x < 0.02f && this->body->GetLinearVelocity().x > -0.02f && this->last_speed.x != this->body->GetLinearVelocity().x) || (this->last_speed.y > 0 && this->body->GetLinearVelocity().y < 0) || (this->last_speed.y < -0.2f && this->body->GetLinearVelocity().y >= -0.05f))
		{
			if (this->bouncing == 0)
			{
				this->stop = true;
				this->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				this->body->SetSleepingAllowed(true);
				this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
				return;
			}
			else
			{
				this->bouncing--;
				this->body->SetTransform(b2Vec2(this->body->GetPosition().x - this->last_speed.x * 0.02f, this->body->GetPosition().y - this->last_speed.y * 0.02f), 0);
			}
		}
		this->last_speed = this->body->GetLinearVelocity();

		if (fabs(this->body->GetLinearVelocity().x) > fabs(this->body->GetLinearVelocity().y))
		{
			if (this->body->GetLinearVelocity().x > 0)
				this->setRotation(180);
			else
				this->setRotation(0);
		}
		else
		{
			if (this->body->GetLinearVelocity().y > 0)
				this->setRotation(270);
			else
				this->setRotation(90);
		}

		//Pozycja
		this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
		this->last_pos = this->getPosition();

		//Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)
		if ((int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32) < ((world_size.x / 32 + 1) * (world_size.y / 32 + 1)) && fluid_tab[(int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32)])
		{
			//Ró¿ne oddzia³ywania zale¿nie od typu p³ynu
			switch (world_type)
			{
			case WORLD_DESERT:
			{
				if (this->gravity)
					this->body->SetGravityScale(0.035f);
				if (this->body->GetLinearVelocity().y > 0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.5f));
				if (this->body->GetLinearVelocity().x > speed.x * g_fluid_speed_multipler.quicksand)
					this->body->SetLinearVelocity(b2Vec2(speed.x * g_fluid_speed_multipler.quicksand, this->body->GetLinearVelocity().y));
				break;
			}
			default:
			{
				if (this->gravity)
					this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 1.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 1.5f));
				if (this->body->GetLinearVelocity().x > speed.x * g_fluid_speed_multipler.water)
					this->body->SetLinearVelocity(b2Vec2(speed.x * g_fluid_speed_multipler.water, this->body->GetLinearVelocity().y));
				break;
			}
			}
		}
		else	//Zwyk³a przestrzeñ (bez p³ynów)
		{
			if (this->gravity)
				this->body->SetGravityScale(1.0f);
		}
		//!Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)
	}
}

void cBullet::specialCollisions(b2World &physics_world, eWorld world_type, bool *modulators, std::vector <cCharacter*> &character, std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cBonusBlock> &bonus_block)
{
	if (!this->stop)
	{
		//Kolizje z Bonusowymi blokami
		for (int i = bonus_block.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(bonus_block[i].getGlobalBounds()))
			{
				this->piercing--;
				if (this->piercing == 0)
					this->destroyed = true;

				if (this->player_id != 0)
					character[this->player_id - 1]->addStatsForBonusBlock();

				bonus_block[i].dropTreasures(physics_world, world_type, treasure, sf::Vector2f((float)((rand() % 2 ? -1 : 1) * rand() % 9) / 10.0f + this->speed.x * 2.25f, -(float)(rand() % 10 + 12) / 10.0f + this->speed.y * 2.25f), modulators);
				bonus_block[i].getBody()->GetWorld()->DestroyBody(bonus_block[i].getBody());
				bonus_block.erase(bonus_block.begin() + i);

				return;
			}
		}

		//Kolizje z NPC-ami
		for (int i = npc.size() - 1; i >= 0; i--)
		{
			if (this->getGlobalBounds().intersects(npc[i].getGlobalBounds()))
			{
				this->piercing--;
				if (this->piercing == 0)
					this->destroyed = true;

				npc[i].hurt(strength);
				if (npc[i].isDead())
				{
					if (this->player_id != 0)
						character[this->player_id - 1]->addStatsForNPC(npc[i]);
					npc.erase(npc.begin() + i);
				}
				
				return;
			}
		}
	}
}

b2Body *cBullet::getBody()
{
	return this->body;
}

bool cBullet::isDestroyed()
{
	return this->destroyed;
}