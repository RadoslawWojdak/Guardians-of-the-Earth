#include "fonts.h"

sf::Font font;

bool initFonts()
{
	if (!font.loadFromFile("fonts\\font-1.ttf"))
		return false;

	return true;
}