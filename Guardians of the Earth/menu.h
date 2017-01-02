#ifndef menu_h
#define menu_h

#include "SFML\Graphics.hpp"
#include <vector>
#include "button.h"
#include "global_variables.h"
#include "character.h"

//bool - czy gracz wybra³ opcjê dalej (w przeciwnym razie menu cofa siê o 1 poziom (z opcji do menu g³ównego; z menu g³ównego wychodzi z gry; ...)
bool mainMenu(sf::RenderWindow &win, short &players);
bool menuChooseNumberOfPlayers(sf::RenderWindow &win, short &players);
bool menuOptions(sf::RenderWindow &win);

bool menuBetweenLevels(sf::RenderWindow &win, std::vector <cCharacter> &player);
bool menuSkillTree(sf::RenderWindow &win, std::vector <cCharacter> &player);

#endif //!menu_h