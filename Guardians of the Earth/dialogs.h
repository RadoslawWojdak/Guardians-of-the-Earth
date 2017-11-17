#ifndef dialogs_h
#define dialogs_h

#include <SFML/Graphics.hpp>
#include "graphics.h"
#include "fonts.h"
#include "button.h"
#include "global_variables.h"
#include "actionOnText.h"

void okDialog(sf::RenderWindow &win, sf::String head, sf::String description, sf::String ok = "OK");
bool yesNoDialog(sf::RenderWindow &win, sf::String head, sf::String description, sf::String yes = "YES", sf::String no = "NO");
std::string textDialog(sf::RenderWindow &win, sf::String head, sf::String description, sf::String ok = "OK", sf::String cancel = "CANCEL");

#endif //!dialogs_h
