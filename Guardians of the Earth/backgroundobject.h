#ifndef backgroundobject_h
#define backgroundobject_h

#include "SFML\Graphics.hpp"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

class cBackgroundObject :public cObjectLevel
{
	eBackgroundType type;		//Szczegó³owy typ obiektu (wiadomo jak go rysowaæ)
	cObjectLevel *extra_sprite;	//Dodatkowe sprite'y (np. w przypadku rysowania drzewa)
	short extra_count;			//Iloœæ dodatkowych sprite'ów

	sf::Texture* randomGraphics(eWorld world_type, sf::Vector2f pos);

public:
	bool front = false;			//Czy grafika ma byæ z przodu? (przed graczem i NPC-ami)

	cBackgroundObject(eWorld world_type, eBackgroundType type, sf::Vector2f pos);

	void setAllPosition(sf::Vector2f pos);
	void drawAllGraphics(sf::RenderWindow &win);

	bool isCollisionOnGrid(bool *object_on_grid_exists, sf::Vector2i grid_size);	//Funkcja zmienionoa pod wzglêdem tej z cObjectLevel (sprawdza jeszcze dodatkowe sprite'y)
};

#endif //!backgroundobject_h