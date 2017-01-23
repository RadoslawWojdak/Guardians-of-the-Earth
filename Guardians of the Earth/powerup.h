#ifndef powerup_h
#define powerup_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"

class cPowerUp :public cObjectLevel
{
	unsigned short power;	//Jaki daje typ mocy

public:
	cPowerUp(sf::Vector2f pos);

	unsigned short getPower();
};

#endif //!powerup_h