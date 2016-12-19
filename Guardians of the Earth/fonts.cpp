#include "fonts.h"

sf::Font font[2];

bool initFonts()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "fonts\\font-" + nr + ".ttf";
		if (!font[i].loadFromFile(path))
			return false;
	}
}