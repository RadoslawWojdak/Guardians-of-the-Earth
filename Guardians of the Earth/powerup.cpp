#include "powerup.h"

cPowerUp::cPowerUp(sf::Vector2f pos)
{
	this->power = rand() % 2 + 1;
	this->adjustGraphicsParameters(t_power_up[this->power - 1], pos);
}

unsigned short cPowerUp::getPower()
{
	return this->power;
}