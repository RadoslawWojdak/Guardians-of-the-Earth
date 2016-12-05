#ifndef graphics_h
#define graphics_h

#include "SFML\Graphics.hpp"
#include <sstream>	//Konwersja liczby na tekst

extern sf::Texture t_block_overworld[2];
extern sf::Texture t_bonus_block_overworld[2];
extern sf::Texture t_ground[2][47];	//Pierwszy wymiar tablicy - rodzaj podloza overground; drugi - ID podloza
extern sf::Texture t_treasure[7];
extern sf::Texture t_background[2][2];
extern sf::Texture t_background_obj[2][9];
extern sf::Texture t_npc[2];
extern sf::Texture t_object[3];

bool init_graph();

#endif //!graphics_h