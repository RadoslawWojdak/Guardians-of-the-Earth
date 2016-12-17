#ifndef pet_h
#define pet_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"

class cPet :public cObjectLevel
{
	sf::Vector2f *point_follow;	//Punkt za którym bêdzie pod¹¿aæ pet

	short hp;
	sf::Vector2f speed;

public:
	cPet();
	cPet(sf::Vector2f *point_follow);

	void move();

	void adjustGraphics();
	void increaseHP();
	void decreaseHP();

	unsigned short getHP();
};

#endif //!pet_h