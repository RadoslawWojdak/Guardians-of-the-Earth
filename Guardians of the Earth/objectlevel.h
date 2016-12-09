#ifndef objectlevel_h
#define objectlevel_h

#include "SFML\Graphics.hpp"
#include "global_variables.h"

struct cObjectLevel :public sf::Sprite
{

public:
	cObjectLevel();
	cObjectLevel(sf::Texture &texture);

	void adjustGraphicsParameters(sf::Texture &texture, sf::Vector2f pos);	//Dostosowanie parametrów grafiki do wymagañ programu (np. origin na œrodku grafiki) - wywo³ywane na sam koniec tworzenia grafiki
	void adjustObjectToLevel(unsigned int map_height);	//Dostosowanie objektu do wysokoœci poziomu (zdjêcia go na sam dó³ poziomu)
	//bool isInView(sf::View &view);	//Spowalnia program - prawdopodobnie klasa View posiada ju¿ podobn¹ funkcjê

	sf::Vector2i posOnGrid(sf::Vector2i grid_rect_size);		//Szerokoœæ i wysokoœæ pojedynczego prostok¹ta siatki - funkcja sprawdza pozycjê obiektu na siatce poziomu ([(0:0), ..., (31:31)] -> (0:0); [(32:0), ..., (63, 31)] -> (1:0), ...)
};

#endif //!objectlevel_h