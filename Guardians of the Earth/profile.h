#ifndef profile_h
#define profile_h

#include "SFML\Graphics.hpp"
#include <string>
#include <fstream>
#include <Windows.h>
#include "dialogs.h"

class cProfile
{
	std::string name;
	unsigned int cash;
	
public:
	cProfile();

	bool newProfile(sf::RenderWindow &win, std::string name);
	bool saveProfile(sf::RenderWindow &win);
	bool loadProfile(sf::RenderWindow &win, std::string name);

	void addCash(unsigned int extra_cash);

	unsigned int getCash();
	std::string getName();
};

#endif //!profile_h