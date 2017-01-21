#ifndef menu_h
#define menu_h

#include "SFML\Graphics.hpp"
#include <vector>
#include <string>
#include "button.h"
#include "global_variables.h"
#include "character.h"
#include "profile.h"
#include "checkbox.h"
#include "enums.h"
#include "dialogs.h"
#include "actionOnText.h"

//bool - czy gracz wybra� opcj� dalej (w przeciwnym razie menu cofa si� o 1 poziom (z opcji do menu g��wnego; z menu g��wnego wychodzi z gry; ...)
bool mainMenu(sf::RenderWindow &win, cProfile &profile, short &players, eCharacter character[], bool *modulators_tab);
bool menuChooseNumberOfPlayers(sf::RenderWindow &win, short &players, bool *modulators_tab);
bool menuSelectCharacters(sf::RenderWindow &win, short players, eCharacter character[], bool *modulators_tab);
bool menuOptions(sf::RenderWindow &win);
bool menuControlSettings(sf::RenderWindow &win);
bool menuProfiles(sf::RenderWindow &win, cProfile &profile);

bool menuBetweenLevels(sf::RenderWindow &win, std::vector <cCharacter*> &player);
bool menuSkillTree(sf::RenderWindow &win, std::vector <cCharacter*> &player);

//bool - czy gracz wyszed� z rozgrywki
bool menuPause(sf::RenderWindow &win);

#endif //!menu_h