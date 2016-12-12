#include "npc.h"
#include <iostream>
cNPC::cNPC(b2World *physics_world, eWorld world_type, unsigned short id, sf::Vector2f pos, eDirection direction)
{
	this->adjustGraphicsParameters(t_npc[id - 1], pos);
	
	this->id = id;
	this->dir = direction;
	this->setFeatures(id);

	if (direction == DIR_LEFT)
		this->last_speed = b2Vec2(-this->speed, 0.5f);	//Poprzednia szybkoœæ pionowa musi byæ wiêksza od zero - wymagane do wyeliminowania bugów dotycz¹cych skakania NPC-ów, które dopiero powsta³y; prêdkoœæ pozioma - wymagana w celu sprawdzenia, czy prêdkoœæ zosta³a zmieniona
	else
		this->last_speed = b2Vec2(this->speed, 0.5f);

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

	//GOTO Gdy NPC-y spadn¹ na siebie, to niszczy siê ich poruszanie siê
	b2PolygonShape shape;	//NPC-y musz¹ byæ wype³nione, gdy¿ pojedyncze krawêdzie nie koliduj¹ ze sob¹ (NPC-y przelatywa³yby przez wszystko co jest stworzone na bazie krawêdzi)
	shape.SetAsBox(a / 2.0f, b / 2.0f - 0.02f);		//Y zmiejszone o 0.02f - teraz NPC mo¿e przechodziæ w szczelinach o swojej wysokoœci
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 20.0f;
	//fd.friction = 1.0f;
	//fd.restitution = 0.0f;

	//NPC - 2; Koliduje z Grunt, Blok, ...
	fd.filter.categoryBits = CATEGORY(CAT_NPC);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | CATEGORY(CAT_NPC) | (world_type == WORLD_ICE_LAND ? CATEGORY(CAT_FLUID) : NULL);

	//body->CreateFixture(shape, 1.0f);
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
	//Odbicie lustrzane grafiki ze wzglêdu na kierunek ruchu
	if (this->dir == DIR_LEFT)
		this->setScale(sf::Vector2f(1, 1));
	else if (this->dir == DIR_RIGHT)
		this->setScale(sf::Vector2f(-1, 1));
	//!Odbicie lustrzane grafiki ze wzglêdu na kierunek ruchu

	if (!start && this->getGlobalBounds().intersects(view_rect))
	{
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
		//Je¿eli NPC ani nie lata, ani nie œciga, to znaczy ¿e ma prêdkoœæ sta³¹ i wystarczy sprawdziæ tylko, czy zosta³a ona w jakikolwiek sposób zmieniona, aby oceniæ, czy NPC w coœ uderzy³
		if (!this->features.flying && !this->features.chase)
		{
			//Je¿eli prêdkoœæ pozioma NPC-a siê zmieni³a to znaczy, ¿e musia³ w coœ uderzyæ swoim bokiem, czyli teraz zmienia siê jego prêdkoœæ na odwrotn¹
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

				//this->body->SetLinearVelocity(b2Vec2(-this->last_speed.x, this->body->GetLinearVelocity().y));
			}
		}

		//Je¿eli NPC lata, ale nie œciga postaci graczy, to po jakimœ czasie siê zatrzymuje i leci w drug¹ stronê
		else if (this->features.flying && !this->features.chase)
		{
			this->setAllPositions(sf::Vector2f(this->body->GetPosition().x * 50, this->last_position.y));	//Pozycja nie bêdzie siê zmienia³a, co naprawia bug dotycz¹cy opadania NPC-a, gdy coœ na niego spadnie

			if (this->dir == DIR_LEFT)
			{
				this->speed -= 0.02f;
				if (this->speed <= -this->features.max_speed)
					this->dir = DIR_RIGHT;
			}
			else if (this->dir == DIR_RIGHT)
			{
				this->speed += 0.02f;
				if (this->speed >= this->features.max_speed)
					this->dir = DIR_LEFT;
			}

			this->body->SetLinearVelocity(b2Vec2(this->speed, 0));
		}

		//Je¿eli NPC bêdzie mia³ wczeœniej prêdkoœæ Y mniejsz¹ od zero, a teraz równ¹ zero, to podskoczy
		if (this->features.jumping)
		{
			if (this->last_speed.y > 0 && this->body->GetLinearVelocity().y <= 0)
				this->body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -7));
		}

		//Kolizje z p³ynami (zmiana grawitacji, oraz prêdkoœci)
		if ((int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32) < ((world_size.x / 32 + 1) * (world_size.y / 32 + 1)) && fluid_tab[(int)((this->getPosition().y - (this->getOrigin().y)) / 32) * (world_size.x / 32) + (int)((this->getGlobalBounds().left + this->getTextureRect().width) / 32)])
		{
			if (this->features.flying && !this->features.swimming)	//Lataj¹ce przestaj¹ lataæ, chyba ¿e potrafi¹ p³ywaæ
			{
				this->features.flying = false;
				this->features.max_speed = 0;
			}
			else if (this->features.swimming && !this->features.chase)	//P³ywaj¹ce, nie œcigaj¹ce przy zderzeniu kolizji z wod¹, zaczynaj¹ p³ywaæ (nie opadaj¹)
			{
				this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.0f));
				this->body->SetGravityScale(0.0f);
			}

			//Ró¿ne oddzia³ywania zale¿nie od typu p³ynu
			switch (world_type)
			{
			case WORLD_DESERT:
			{
				if (!this->features.swimming)
					this->body->SetGravityScale(0.035f);
				if (this->body->GetLinearVelocity().y > 0.5f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 0.5f));
				if (this->body->GetLinearVelocity().x > (float)this->features.max_speed / 3.5f)
				{
					this->speed = (float)this->features.max_speed / 3.5f;
					this->body->SetLinearVelocity(b2Vec2((float)this->features.max_speed / 3.5f, this->body->GetLinearVelocity().y));
				}
				else if (this->body->GetLinearVelocity().x < -(float)this->features.max_speed / 3.5f)
				{
					this->speed = -(float)this->features.max_speed / 3.5f;
					this->body->SetLinearVelocity(b2Vec2(-(float)this->features.max_speed / 3.5f, this->body->GetLinearVelocity().y));
				}

				break;
			}
			default:
			{
				if (!this->features.swimming)
					this->body->SetGravityScale(0.35f);
				if (this->body->GetLinearVelocity().y > 2.0f)
					this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x, 2.0f));
				if (this->body->GetLinearVelocity().x > (float)this->features.max_speed / 2.0f)
				{
					this->speed = (float)this->features.max_speed / 2.0f;
					this->body->SetLinearVelocity(b2Vec2((float)this->features.max_speed / 2.0f, this->body->GetLinearVelocity().y));
				}
				else if (this->body->GetLinearVelocity().x < -(float)this->features.max_speed / 2.0f)
				{
					this->speed = -(float)this->features.max_speed / 2.0f;
					this->body->SetLinearVelocity(b2Vec2(-(float)this->features.max_speed / 2.0f, this->body->GetLinearVelocity().y));
				}

				break;
			}
			}
		}
		else	//Zwyk³a przestrzeñ (bez p³ynów)
		{
			if (!this->features.flying)
			this->body->SetGravityScale(1.0f);

			if (!this->features.chase && !this->features.flying)	//Je¿eli porusza siê ze sta³¹ prêdkoœci¹
			{
				if (this->dir == DIR_LEFT)
					this->body->SetLinearVelocity(b2Vec2(-this->features.max_speed, this->body->GetLinearVelocity().y));
				else
					this->body->SetLinearVelocity(b2Vec2(this->features.max_speed, this->body->GetLinearVelocity().y));
			}
		}
		
		//Ustawienie ostatniej pozycji musi byæ w tym miejscu, gdy¿ póŸniej zostanie zmieniona zarówno przez czynniki z tej funkcji, jak i kolizje
		this->last_speed = this->body->GetLinearVelocity();
	}

	//Pozycja
	this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
	this->last_position = this->getPosition();
}
/*
void cNPC::startMoving()
{
	if (this->motion)
	{
		if (this->dir == DIR_LEFT)
			body->SetLinearVelocity(b2Vec2(-this->speed, 0));
		else if (this->dir == DIR_RIGHT)
			body->SetLinearVelocity(b2Vec2(this->speed, 0));
	}
}
*/
void cNPC::setAllPositions(sf::Vector2f pos)
{
	//this->adjustGraphicsParameters(t_npc[this->id - 1], pos);
	this->body->SetTransform(b2Vec2(pos.x * 0.02f, pos.y * 0.02f), 0);
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
	last_position = pos;
}

void cNPC::moveAllPositions(sf::Vector2f pos)
{
	this->move(pos);
	this->body->SetLinearVelocity(b2Vec2(pos.x, this->body->GetLinearVelocity().y));
	//this->body->SetTransform(b2Vec2(this->getPosition().x * 0.02f, this->getPosition().y * 0.02f), 0);
}

bool cNPC::isSolidCollision(bool *solid_object, bool *npc_exists, sf::Vector2i grid_size)
{
	sf::Vector2i pos_on_grid = this->posOnGrid(sf::Vector2i(32, 32));//((this->getPosition().x - 32) / 32, (this->getPosition().y - 32) / 32);

	//Czy wysz³o poza zakres? (poza ramy poziomu)
	if (pos_on_grid.x < 0 || pos_on_grid.x >= grid_size.x || pos_on_grid.y < 0 || pos_on_grid.y >= grid_size.y)
		return false;
	//Sztywne obiekty
	if (solid_object[pos_on_grid.y * grid_size.x + pos_on_grid.x])
		return true;
	//NPC-y
	if (npc_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
		return true;

	return false;
}

b2Body* cNPC::getBody()
{
	return this->body;
}

sFeatures cNPC::getFeatures()
{
	return this->features;
}