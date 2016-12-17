#include "npc.h"
#include <iostream>
cNPC::cNPC(b2World *physics_world, eWorld world_type, unsigned short id, sf::Vector2f pos, eDirection direction)
{
	this->adjustGraphicsParameters(t_npc[id - 1], pos);
	
	this->id = id;
	this->dir = direction;
	this->setFeatures(id);

	if (direction == DIR_LEFT)
		this->last_speed = b2Vec2(-this->speed, 0.5f);	//Poprzednia szybko�� pionowa musi by� wi�ksza od zero - wymagane do wyeliminowania bug�w dotycz�cych skakania NPC-�w, kt�re dopiero powsta�y; pr�dko�� pozioma - wymagana w celu sprawdzenia, czy pr�dko�� zosta�a zmieniona
	else
		this->last_speed = b2Vec2(this->speed, 0.5f);

	this->dead = false;

	//BOX2D
	float32 a = this->getTextureRect().width * 0.02f;
	float32 b = this->getTextureRect().height * 0.02f;

	body_def.position.Set(pos.x * 0.02f, pos.y * 0.02f);
	body_def.type = b2_dynamicBody;
	body_def.fixedRotation = true;
	body_def.allowSleep = false;

	this->body = physics_world->CreateBody(&body_def);
	this->body->SetActive(false);
	if (this->features.flying)
		this->body->SetGravityScale(0.0f);


	b2PolygonShape shape;	//NPC-y musz� by� wype�nione, gdy� pojedyncze kraw�dzie nie koliduj� ze sob� (NPC-y przelatywa�yby przez wszystko co jest stworzone na bazie kraw�dzi)
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.02f);		//Y zmiejszone o 0.02f - teraz NPC mo�e przechodzi� w szczelinach o swojej wysoko�ci
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 20.0f;

	//NPC - 2; Koliduje z Grunt, Blok, ...
	fd.filter.categoryBits = CATEGORY(CAT_NPC);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | CATEGORY(CAT_NPC) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	body->CreateFixture(&fd);
}

void cNPC::setFeatures(unsigned short id)
{
	switch(id)
	{
	case 1:
	{
		this->setTextureRect(sf::IntRect(0, 0, 32, 32));

		this->features.friendly = false;
		
		this->features.motion = true;
		this->features.chase = false;
		this->speed = 1.5f;
		this->features.max_speed = 1.5f;

		this->features.flying = false;
		this->features.swimming = false;
		this->features.jumping = false;
		
		break;
	}
	case 2:
	{
		this->setTextureRect(sf::IntRect(0, 0, 32, 32));

		this->features.friendly = false;

		this->features.motion = true;
		this->features.chase = false;
		this->speed = 1.5f;
		this->features.max_speed = 1.5f;

		this->features.flying = false;
		this->features.swimming = false;
		this->features.jumping = true;

		break;
	}
	case 3:
	{
		this->setTextureRect(sf::IntRect(0, 0, 32, 32));

		this->features.friendly = false;

		this->features.motion = true;
		this->features.chase = false;
		this->speed = 2.5f;
		this->features.max_speed = 2.5f;

		this->features.flying = true;
		this->features.swimming = false;
		this->features.jumping = false;

		break;
	}
	case 4:
	{
		this->setTextureRect(sf::IntRect(0, 0, 32, 32));

		this->features.friendly = false;

		this->features.motion = true;
		this->features.chase = false;
		this->speed = 2.5f;
		this->features.max_speed = 2.5f;

		this->features.flying = false;
		this->features.swimming = true;
		this->features.jumping = false;

		break;
	}
	}
}

void cNPC::step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab, sf::FloatRect &view_rect)
{
	float speed_multipler = 1;
	switch (this->is_immersed_in)
	{
	case FLUID_WATER: {speed_multipler = g_fluid_speed_multipler.water; break;}
	case FLUID_QUICKSAND: {speed_multipler = g_fluid_speed_multipler.quicksand; break;}
	}

	//Odbicie lustrzane grafiki ze wzgl�du na kierunek ruchu
	if (this->dir == DIR_LEFT)
		this->setScale(sf::Vector2f(1, 1));
	else if (this->dir == DIR_RIGHT)
		this->setScale(sf::Vector2f(-1, 1));
	//!Odbicie lustrzane grafiki ze wzgl�du na kierunek ruchu

	if (!start && this->getGlobalBounds().intersects(view_rect))
	{
		this->last_position = this->getPosition();	//Naprawa b��d�w z NPC-ami kt�re pojawiaj� si� ju� na samym poczatku rozgrywki
		this->body->SetActive(true);
		this->start = true;
		
		if (this->features.motion)
		{
			if (this->dir == DIR_LEFT)
				this->speed = -this->features.max_speed;
			else if (this->dir == DIR_RIGHT)
				this->speed = this->features.max_speed;
		}
	}
	if (this->start && this->features.motion)
	{
		//Je�eli NPC ani nie lata, ani nie �ciga, to znaczy �e ma pr�dko�� sta�� i wystarczy sprawdzi� tylko, czy zosta�a ona w jakikolwiek spos�b zmieniona, aby oceni�, czy NPC w co� uderzy�
		if (!this->features.flying && !this->features.chase)
		{
			//Je�eli pr�dko�� pozioma NPC-a si� zmieni�a to znaczy, �e musia� w co� uderzy� swoim bokiem, czyli teraz zmienia si� jego pr�dko�� na odwrotn�
			if (this->last_speed.x != this->body->GetLinearVelocity().x)
			{
				if (last_speed.x > 0)
				{
					this->dir = DIR_LEFT;
					this->body->SetLinearVelocity(b2Vec2(-this->features.max_speed, this->body->GetLinearVelocity().y));
				}
				else
				{
					this->dir = DIR_RIGHT;
					this->body->SetLinearVelocity(b2Vec2(this->features.max_speed, this->body->GetLinearVelocity().y));
				}
			}
		}

		//Je�eli NPC lata, ale nie �ciga postaci graczy, to po jakim� czasie si� zatrzymuje i leci w drug� stron�
		else if (this->features.flying && !this->features.chase)
		{
			this->setAllPositions(sf::Vector2f(this->body->GetPosition().x * 50, this->last_position.y));	//Pozycja nie b�dzie si� zmienia�a, co naprawia bug dotycz�cy opadania NPC-a, gdy co� na niego spadnie

			if (this->dir == DIR_LEFT)
			{
				this->speed -= 0.02f * speed_multipler;
				if (this->speed <= -this->features.max_speed * speed_multipler)
					this->dir = DIR_RIGHT;
			}
			else if (this->dir == DIR_RIGHT)
			{
				this->speed += 0.02f * speed_multipler;
				if (this->speed >= this->features.max_speed * speed_multipler)
					this->dir = DIR_LEFT;
			}

			this->body->SetLinearVelocity(b2Vec2(this->speed, 0));
		}

		//Je�eli NPC b�dzie mia� wcze�niej pr�dko�� Y mniejsz� lub r�wn� od zero, a teraz r�wn� zero, to podskoczy
		if (this->features.jumping)
		{
			if (this->last_speed.y >= 0 && this->body->GetLinearVelocity().y <= 0)
				this->body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -7));
		}

		//Kolizje z p�ynami (zmiana grawitacji, oraz pr�dko�ci)
		if ((int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32) < ((world_size.x / 32 + 1) * (world_size.y / 32 + 1)) && fluid_tab[(int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32)])
		{
			if (this->features.flying && !this->features.swimming)	//Lataj�ce przestaj� lata�, chyba �e potrafi� p�ywa�
			{
				this->features.flying = false;
				this->features.max_speed = 0;
			}
			else if (this->features.swimming && !this->features.chase)	//P�ywaj�ce, nie �cigaj�ce przy zderzeniu kolizji z wod�, zaczynaj� p�ywa� (nie opadaj�)
			{
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));
				this->body->SetGravityScale(0.0f);
			}

			//R�ne oddzia�ywania zale�nie od typu p�ynu
			switch (world_type)
			{
			case WORLD_DESERT:
			{
				if (!this->features.swimming)
					this->body->SetGravityScale(0.035f);
				if (this->body->GetLinearVelocity().y > 0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.5f));
				if (this->body->GetLinearVelocity().x > (float)this->features.max_speed * speed_multipler)
				{
					this->speed = (float)this->features.max_speed * speed_multipler;
					this->body->SetLinearVelocity(b2Vec2((float)this->features.max_speed * speed_multipler, this->body->GetLinearVelocity().y));
				}
				else if (this->body->GetLinearVelocity().x < -(float)this->features.max_speed * speed_multipler)
				{
					this->speed = -(float)this->features.max_speed * speed_multipler;
					this->body->SetLinearVelocity(b2Vec2(-(float)this->features.max_speed * speed_multipler, this->body->GetLinearVelocity().y));
				}

				break;
			}
			default:
			{
				if (!this->features.swimming)
					this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 2.0f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 2.0f));
				if (this->body->GetLinearVelocity().x > (float)this->features.max_speed * speed_multipler)
				{
					this->speed = (float)this->features.max_speed * speed_multipler;
					this->body->SetLinearVelocity(b2Vec2((float)this->features.max_speed * speed_multipler, this->body->GetLinearVelocity().y));
				}
				else if (this->body->GetLinearVelocity().x < -(float)this->features.max_speed * speed_multipler)
				{
					this->speed = -(float)this->features.max_speed * speed_multipler;
					this->body->SetLinearVelocity(b2Vec2(-(float)this->features.max_speed * speed_multipler, this->body->GetLinearVelocity().y));
				}

				break;
			}
			}
		}
		else	//Zwyk�a przestrze� (bez p�yn�w)
		{
			if (!this->features.flying)
			this->body->SetGravityScale(1.0f);

			if (!this->features.chase && !this->features.flying)	//Je�eli porusza si� ze sta�� pr�dko�ci�
			{
				if (this->dir == DIR_LEFT)
					this->body->SetLinearVelocity(b2Vec2(-this->features.max_speed, this->body->GetLinearVelocity().y));
				else
					this->body->SetLinearVelocity(b2Vec2(this->features.max_speed, this->body->GetLinearVelocity().y));
			}
		}
		
		//Ustawienie ostatniej pozycji musi by� w tym miejscu, gdy� p�niej zostanie zmieniona zar�wno przez czynniki z tej funkcji, jak i kolizje
		this->last_speed = this->body->GetLinearVelocity();
	}

	//Pozycja
	this->setAllPositions(sf::Vector2f(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f));
}

void cNPC::kill()
{
	if (!this->isDead())
	{
		this->body->GetWorld()->DestroyBody(this->body);
		this->dead = true;
	}
}

void cNPC::setAllPositions(sf::Vector2f pos)
{
	this->last_position = this->getPosition();
	this->body->SetTransform(b2Vec2(pos.x * 0.02f, pos.y * 0.02f), 0);
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}

b2Body* cNPC::getBody()
{
	return this->body;
}

sFeatures cNPC::getFeatures()
{
	return this->features;
}

sf::Vector2f cNPC::getLastPosition()
{
	return this->last_position;
}

bool cNPC::isDead()
{
	return this->dead;
}