#ifndef backgroundobject_h
#define backgroundobject_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

class cBackgroundObject :public cObjectLevel
{
	eType type;
	cObjectLevel *extra_sprite;	//Dodatkowe sprit'y (np. w przypadku rysowania drzewa)
	short extra_count;			//Iloœæ dodatkowych sprit'ów

public:
	cBackgroundObject(eType type, sf::Vector2f pos);

	sf::Texture* randomGraphics(sf::Vector2f pos);

	void setAllPosition(sf::Vector2f pos);

	void drawAllGraphics(sf::RenderWindow &win);

	bool isGroundCollision(bool *ground_exists, sf::Vector2i grid_size);
	bool isWaterCollision(bool *water_exists, sf::Vector2i grid_size);
};

#endif //!backgroundobject_h