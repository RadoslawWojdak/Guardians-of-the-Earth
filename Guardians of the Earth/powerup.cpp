#include "powerup.h"

cPowerUp::cPowerUp(sf::Vector2f pos)
{
	this->adjustGraphicsParameters(t_power_up[0], pos);
}