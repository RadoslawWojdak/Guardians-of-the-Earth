#ifndef menu_h
#define menu_h

#include "SFML\Graphics.hpp"
#include "button.h"
#include "global_variables.h"

//bool - czy gracz wybra� opcj� dalej (w przeciwnym razie menu cofa si� o 1 poziom (z opcji do menu g��wnego; z menu g��wnego wychodzi z gry; ...)
bool mainMenu(sf::RenderWindow &win, short &players);
bool menuChooseNumerOfPlayers(sf::RenderWindow &win, short &players);


#endif //!menu_h