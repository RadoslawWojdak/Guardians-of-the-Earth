#ifndef backgroundobject_h
#define backgroundobject_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

class cBackgroundObject :public cObjectLevel
{
	//eType type;				//Uogólniony typ obiektu (wiadomo gdzie mo¿e siê poruszaæ)
	eBackgroundType type;		//Szczegó³owy typ obiektu (wiadomo jak go rysowaæ)
	cObjectLevel *extra_sprite;	//Dodatkowe sprit'y (np. w przypadku rysowania drzewa)
	short extra_count;			//Iloœæ dodatkowych sprit'ów

	//void adjustBgType();
	sf::Texture* randomGraphics(eWorld world_type, sf::Vector2f pos);

public:
	bool front = false;			//Czy grafika ma byæ z przodu? (przed graczem i NPC-ami)

	cBackgroundObject(eWorld world_type, eBackgroundType type, sf::Vector2f pos);

	void setAllPosition(sf::Vector2f pos);

	void drawAllGraphics(sf::RenderWindow &win);

	bool isGroundCollision(bool *ground_exists, sf::Vector2i grid_size);
	bool isWaterCollision(bool *water_exists, sf::Vector2i grid_size);
};

#endif //!backgroundobject_h