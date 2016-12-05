#include "npc.h"
#include <iostream>
cNPC::cNPC(b2World *physics_world, unsigned short id, sf::Vector2f pos, eDirection direction)
{
	this->adjustGraphicsParameters(t_npc[id - 1], pos);
	
	this->id = id;
	this->dir = direction;
	this->setFeatures(id);

	if (direction == DIR_LEFT)
		this->last_speed = b2Vec2(-this->speed, 0.5f);	//Poprzednia szybko�� pionowa musi by� wi�ksza od zero - wymagane do wyeliminowania bug�w dotycz�cych skakania NPC-�w, kt�re dopiero powsta�y; pr�dko�� pozioma - wymagana w celu sprawdzenia, czy pr�dko�� zosta�a zmieniona
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
	if (flying)
		this->body->SetGravityScale(0.0f);

	//GOTO Gdy NPC-y spadn� na siebie, to niszczy si� ich poruszanie si�
	b2PolygonShape shape;	//NPC-y musz� by� wype�nione, gdy� pojedyncze kraw�dzie nie koliduj� ze sob� (NPC-y przelatywa�yby przez wszystko co jest stworzone na bazie kraw�dzi)
	shape.SetAsBox(a / 2.0f, b / 2.0f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 20.0f;
	//fd.friction = 1.0f;
	//fd.restitution = 0.0f;

	//NPC - 2; Koliduje z Grunt, Blok, ...
	fd.filter.categoryBits = CATEGORY(CAT_NPC);
	fd.filter.maskBits = CATEGORY(CAT_GROUND) | CATEGORY(CAT_BLOCK) | CATEGORY(CAT_BONUS_BLOCK) | CATEGORY(CAT_NPC);

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

		this->friendly = false;
		
		this->motion = true;
		this->chase = false;
		this->speed = 1.5f;
		this->max_speed = 1.5f;

		this->flying = false;
		this->swimming = false;
		this->jumping = false;
		
		break;
	}
	}
}

void cNPC::step(sf::FloatRect &view_rect)
{
	if (!start && this->getGlobalBounds().intersects(view_rect))
	{
		this->body->SetActive(true);
		start = true;
		
		if (this->motion)
		{
			if (this->dir == DIR_LEFT)
				speed = -max_speed;
			else if (this->dir == DIR_RIGHT)
				speed = max_speed;
		}
	}
	if (this->start && this->motion)
	{
		//Je�eli NPC ani nie lata, ani nie �ciga, to znaczy �e ma pr�dko�� sta�� i wystarczy sprawdzi� tylko, czy zosta�a ona w jakikolwiek spos�b zmieniona, aby oceni�, czy NPC w co� uderzy�
		if (!this->flying && !this->chase)
		{
			//Je�eli pr�dko�� pozioma NPC-a si� zmieni�a to znaczy, �e musia� w co� uderzy� swoim bokiem, czyli teraz zmienia si� jego pr�dko�� na odwrotn�
			if (this->last_speed.x != this->body->GetLinearVelocity().x)
			{
				this->body->SetLinearVelocity(b2Vec2(-this->last_speed.x, this->body->GetLinearVelocity().y));
			}
		}

		//Je�eli NPC lata, ale nie �ciga postaci graczy, to po jakim� czasie si� zatrzymuje i leci w drug� stron�
		else if (this->flying && !this->chase)
		{
			if (this->dir == DIR_LEFT)
			{
				this->speed -= 0.01f;
				if (this->speed <= -this->max_speed)
					this->dir = DIR_RIGHT;
			}
			else if (this->dir == DIR_RIGHT)
			{
				this->speed += 0.01f;
				if (this->speed >= this->max_speed)
					this->dir = DIR_LEFT;
			}
		}

		//Je�eli NPC b�dzie mia� wcze�niej pr�dko�� Y mniejsz� od zero, a teraz r�wn� zero, to podskoczy
		if (this->jumping)
		{
			if (this->last_speed.y > 0 && this->body->GetLinearVelocity().y <= 0)
				this->body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -7));
		}

		
		//Ustawienie ostatniej pozycji musi by� w tym miejscu, gdy� p�niej zostanie zmieniona zar�wno przez czynniki z tej funkcji, jak i kolizje
		this->last_speed = this->body->GetLinearVelocity();
	}

	//Pozycja
	this->setPosition(this->body->GetPosition().x * 50.0f, this->body->GetPosition().y * 50.0f);
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
}

void cNPC::moveAllPositions(sf::Vector2f pos)
{
	this->move(pos);
	this->body->SetLinearVelocity(b2Vec2(pos.x, this->body->GetLinearVelocity().y));
	//this->body->SetTransform(b2Vec2(this->getPosition().x * 0.02f, this->getPosition().y * 0.02f), 0);
}

bool cNPC::isSolidCollision(std::vector <cGround> &ground, std::vector <cBlock> &block, std::vector <cBonusBlock> &bonus_block, std::vector <cNPC> &npc)
{
	for (unsigned int i = 0; i < ground.size(); i++)
		if (ground[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
	for (unsigned int i = 0; i < block.size(); i++)
		if (block[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
	for (unsigned int i = 0; i < bonus_block.size(); i++)
		if (bonus_block[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
	for (unsigned int i = 0; i < npc.size(); i++)
		if (npc[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
	
	return false;
}

b2Body* cNPC::getBody()
{
	return this->body;
}