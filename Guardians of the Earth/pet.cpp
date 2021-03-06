#include "pet.h"

cPet::cPet()
{
	;
}

cPet::cPet(eCharacter character_type, sf::Vector2f *point_follow, short initial_hp)
{
	this->point_follow = point_follow;
	this->adjustGraphicsParameters(t_pet[character_type], sf::Vector2f(this->point_follow->x - 4.0f, this->point_follow->y - 16.0f));
	
	this->hp = initial_hp;
	if (this->hp < 0)
		this->hp = 0;
	else if (this->hp > 3)
		this->hp = 3;

	this->adjustGraphics();
}

void cPet::move()
{
	this->speed.x = (point_follow->x - this->getPosition().x) / 25;
	if (speed.x > 3.0f)
		speed.x = 3.0f;
	else if (speed.x < -3.0f)
		speed.x = -3.0f;

	this->speed.y = (point_follow->y - this->getPosition().y) / 25;
	if (speed.y > 3.0f)
		speed.y = 3.0f;
	else if (speed.y < -3.0f)
		speed.y = -3.0f;

	this->cObjectLevel::move(speed);
}

void cPet::adjustGraphics()
{
	if (this->hp == 1)
		this->setTextureRect(sf::IntRect(0, 0, this->getTexture()->getSize().x, this->getTexture()->getSize().y / 3.0f));
	else if (this->hp == 2)
		this->setTextureRect(sf::IntRect(0, this->getTexture()->getSize().y / 3.0f, this->getTexture()->getSize().x, this->getTexture()->getSize().y / 3.0f));
	else if (this->hp == 3)
		this->setTextureRect(sf::IntRect(0, this->getTexture()->getSize().y * (2.0f / 3.0f), this->getTexture()->getSize().x, this->getTexture()->getSize().y / 3.0f));
}

void cPet::increaseHP()
{
	if (this->hp == 0)
		this->setPosition(*(this->point_follow));

	this->hp++;
	if (this->hp > 3)
		this->hp = 3;

	this->adjustGraphics();
}

void cPet::decreaseHP()
{
	this->hp--;
	if (this->hp < 0)
		this->hp = 0;

	this->adjustGraphics();
}

void cPet::kill()
{
	this->hp = 0;
}

unsigned short cPet::getHP()
{
	return this->hp;
}