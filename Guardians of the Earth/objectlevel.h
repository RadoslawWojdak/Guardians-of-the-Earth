#ifndef objectlevel_h
#define objectlevel_h

#include "SFML\Graphics.hpp"
#include "global_variables.h"

struct cObjectLevel :public sf::Sprite
{

public:
	cObjectLevel();
	cObjectLevel(sf::Texture &texture);

	void adjustGraphicsParameters(sf::Texture &texture, sf::Vector2f pos);	//Dostosowanie parametr�w grafiki do wymaga� programu (np. origin na �rodku grafiki) - wywo�ywane na sam koniec tworzenia grafiki
	void adjustObjectToLevel(unsigned int map_height);	//Dostosowanie objektu do wysoko�ci poziomu (zdj�cia go na sam d� poziomu)
	bool isCollisionOnGrid(bool *object_on_grid_exists, sf::Vector2i grid_size);

	sf::Vector2i posOnGrid(sf::Vector2i grid_rect_size);		//Szeroko�� i wysoko�� pojedynczego prostok�ta siatki - funkcja sprawdza pozycj� obiektu na siatce poziomu ([(0:0), ..., (31:31)] -> (0:0); [(32:0), ..., (63, 31)] -> (1:0), ...)
};

#endif //!objectlevel_h