#ifndef backgroundobject_h
#define backgroundobject_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"
#include "ground.h"
#include "water.h"

class cBackgroundObject :public cObjectLevel
{
	eType type;
	sf::Sprite *extra_sprite;	//Dodatkowe sprit'y (np. w przypadku rysowania drzewa)
	short extra_count;			//Iloœæ dodatkowych sprit'ów

public:
	cBackgroundObject(eType type, sf::Vector2f pos);

	sf::Texture* randomGraphics(sf::Vector2f pos);

	void setAllPosition(sf::Vector2f pos);

	void drawAllGraphics(sf::RenderWindow &win);

	bool isGroundCollision(std::vector <cGround> &ground);
	bool isWaterCollision(std::vector <cWater> &water);
};

#endif //!backgroundobject_h