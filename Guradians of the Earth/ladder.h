#ifndef ladder_h
#define ladder_h

#include "SFML/Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"

class cLadder :public cObjectLevel
{
public:
	cLadder(sf::Vector2f pos);
};

#endif //!ladder_h