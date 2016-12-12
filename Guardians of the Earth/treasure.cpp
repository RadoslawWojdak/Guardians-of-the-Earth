#include "treasure.h"
#include <iostream>
#include <cstdlib>

cTreasure::cTreasure(b2World *physics_world, sf::Vector2f pos, bool physics, float speed_x, float speed_y)
{
	//Losowanie wartoœci skaru
	clock_t t1;
	t1 = clock();

	static unsigned long long rnd = t1;
	srand(rnd);
	rnd = rand() % 1844674407370955161 + t1 + ((int)pos.x % (int)pos.y) * 42;

	int nr = rand() % 1001;

	if (nr <= 575)
		this->value = 1;
	else if (nr <= 775)
		this->value = 2;
	else if (nr <= 890)
		this->value = 3;
	else if (nr <= 961)
		this->value = 5;
	else if (nr <= 984)
		this->value = 10;
	else if (nr <= 996)
		this->value = 20;
	else
		this->value = -1;

	//Przypisanie grafiki skarbowi ze wzglêdu na jego wartoœæ
	switch (this->value)
	{
	case -1:
		this->adjustGraphicsParameters(t_treasure[5], pos);
		break;
	case 1:
		this->adjustGraphicsParameters(t_treasure[2], pos);
		break;
	case 2:
		this->adjustGraphicsParameters(t_treasure[1], pos);
		break;
	case 3:
		this->adjustGraphicsParameters(t_treasure[0], pos);
		break;
	case 5:
		this->adjustGraphicsParameters(t_treasure[6], pos);
		break;
	case 10:
		this->adjustGraphicsParameters(t_treasure[4], pos);
		break;
	case 20:
		this->adjustGraphicsParameters(t_treasure[3], pos);
		break;
	default:
		this->adjustGraphicsParameters(t_treasure[0], pos);
		break;
	}

	this->physics = physics;
	//this->speed = sf::Vector2f(speed_x, speed_y);


	//BOX2D
	uint16 category_bits = CATEGORY(CAT_TREASURE);	//Filtr kateogri
	uint16 mask_bits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK);		//Filtr kolizji
	if (physics)
	{
		switch (this->value)
		{
		case T_COPPER_COIN:
		case T_SILVER_COIN:
		case T_GOLDEN_COIN:
		{
			float r = (this->getTextureRect().height * 0.02f) / 2.0f;

			body_def.type = b2_dynamicBody;
			body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
			body_def.fixedRotation = true;
			body_def.allowSleep = true;
			//body_def.angularDamping = 0.7f;
			body_def.linearDamping = 0.55f;

			body = physics_world->CreateBody(&body_def);
			body->SetLinearVelocity(b2Vec2(speed_x, speed_y));

			b2CircleShape shape;
			shape.m_radius = r;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.4f;

			//Skarb - 1; Koliduje z grunt - 0
			fd.filter.categoryBits = category_bits;
			fd.filter.maskBits = mask_bits;

			body->CreateFixture(&fd);
			break;
		}
		case T_HEART:
		{
			float a = (this->getTextureRect().width * 0.02f);
			float b = (this->getTextureRect().height * 0.02f);

			body_def.type = b2_dynamicBody;
			body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
			body_def.fixedRotation = true;
			body_def.allowSleep = false;
			//body_def.linearDamping = 0.15f;

			body = physics_world->CreateBody(&body_def);
			body->SetLinearVelocity(b2Vec2(speed_x, speed_y));

			b2PolygonShape shape;
			shape.SetAsBox((float)a / 2.0f, (float)b / 2.0f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.4f;
			fd.restitution = 1.0f;

			//Skarb - 1; Koliduje z grunt - 0
			fd.filter.categoryBits = category_bits;
			fd.filter.maskBits = mask_bits;

			body->CreateFixture(&fd);

			break;
		}
		default:
		{
			float a = (this->getTextureRect().width * 0.02f);
			float b = (this->getTextureRect().height * 0.02f);

			body_def.type = b2_dynamicBody;
			body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
			body_def.fixedRotation = true;
			body_def.allowSleep = true;
			body_def.linearDamping = 0.8f;

			body = physics_world->CreateBody(&body_def);
			body->SetLinearVelocity(b2Vec2(speed_x, speed_y));

			b2PolygonShape shape;
			shape.SetAsBox((float)a / 2.0f, (float)b / 2.0f);
			/*sf::Vector2i size(this->getTextureRect().width, this->getTextureRect().height);
			const int vxNum = 3;
			b2Vec2 vx[3] = { b2Vec2(0, (size.y * 0.02f) / 2.0f), b2Vec2((size.x * 0.02f - 0.03f) / 2.0f, 0), b2Vec2((size.x * 0.02f) / 2.0f, 0.03f) };
			shape.Set(vx, vxNum);
			*/
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 1.0f;

			//Skarb - 1; Koliduje z grunt - 0
			fd.filter.categoryBits = category_bits;
			fd.filter.maskBits = mask_bits;

			body->CreateFixture(&fd);

			break;
		}
		}
	}
}

void cTreasure::step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab)
{
	if (this->physics)
	{
		//Pozycja
		this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
		
		//Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)
		if ((int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32) < ((world_size.x / 32 + 1) * (world_size.y / 32 + 1)) && fluid_tab[(int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32)])
		{
			//Ró¿ne oddzia³ywania zale¿nie od typu p³ynu
			switch (world_type)
			{
			case WORLD_DESERT:
			{
				this->body->SetGravityScale(0.035f);
				if (this->body->GetLinearVelocity().y > 0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.5f));
				if (this->body->GetLinearVelocity().x > 0.75f)
					this->body->SetLinearVelocity(b2Vec2(0.75f, this->body->GetLinearVelocity().y));
				break;
			}
			default:
			{
				this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 1.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 1.5f));
				if (this->body->GetLinearVelocity().x > 2.0f)
					this->body->SetLinearVelocity(b2Vec2(2.0f, this->body->GetLinearVelocity().y));
				break;
			}
			}
		}
		else	//Zwyk³a przestrzeñ (bez p³ynów)
		{
			this->body->SetGravityScale(1.0f);
		}
		//!Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)


		//K¹t (obrót)
		if ((this->value == T_COPPER_COIN) || (this->value == T_SILVER_COIN) || (this->value == T_GOLDEN_COIN))
		{
			this->body->SetAngularVelocity(this->body->GetLinearVelocity().x);
			this->setRotation(180 * this->body->GetAngle() * 3.14);
		}
	}
}