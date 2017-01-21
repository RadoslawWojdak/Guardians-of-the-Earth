#ifndef dialogs_h
#define dialogs_h

#include <SFML\Graphics.hpp>
#include "graphics.h"
#include "fonts.h"
#include "button.h"
#include "global_variables.h"
#include "actionOnText.h"

bool yesNoDialog(sf::RenderWindow &win, sf::String head, sf::String description);
std::string textDialog(sf::RenderWindow &win, sf::String head, sf::String description);

#endif //!dialogs_h