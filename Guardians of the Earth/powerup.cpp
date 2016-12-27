#include "powerup.h"

cPowerUp::cPowerUp(sf::Vector2f pos)
{
	this->adjustGraphicsParameters(t_power_up[0], pos);
	this->power = 1;
}

unsigned short cPowerUp::getPower()
{
	return this->power;
}