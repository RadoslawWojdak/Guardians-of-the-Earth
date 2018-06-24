#ifndef pet_h
#define pet_h

#include "SFML/Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

class cPet :public cObjectLevel
{
	sf::Vector2f *point_follow;	//Punkt za którym bêdzie pod¹¿aæ pet

	short hp;
	sf::Vector2f speed;

public:
	cPet();
	cPet(eCharacter character_type, sf::Vector2f *point_follow, short initial_hp);

	void move();

	void adjustGraphics();
	void increaseHP();
	void decreaseHP();
	void kill();

	unsigned short getHP();
};

#endif //!pet_h
