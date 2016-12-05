#ifndef objectlevel_h
#define objectlevel_h

#include "SFML\Graphics.hpp"
#include "global_variables.h"

struct cObjectLevel :public sf::Sprite
{


public:
	void adjustGraphicsParameters(sf::Texture &texture, sf::Vector2f pos);	//Dostosowanie parametr�w grafiki do wymaga� programu (np. origin na �rodku grafiki) - wywo�ywane na sam koniec tworzenia grafiki
	void adjustObjectToLevel(unsigned int map_height);	//Dostosowanie objektu do wysoko�ci poziomu (zdj�cia go na sam d� poziomu)
	//bool isInView(sf::View &view);	//Spowalnia program - prawdopodobnie klasa View posiada ju� podobn� funkcj�
};

#endif //!objectlevel_h