#ifndef profile_h
#define profile_h

#include "SFML\Graphics.hpp"
#include <string>
#include <fstream>
#include <Windows.h>
#include "dialogs.h"
#include "enums.h"

class cProfile
{
	std::string name;
	unsigned int cash;

	bool **unlocked_content;
	
public:
	cProfile();

	bool newProfile(sf::RenderWindow &win, std::string name);
	bool saveProfile(sf::RenderWindow &win);
	bool loadProfile(sf::RenderWindow &win, std::string name);

	void addCash(unsigned int extra_cash);
	bool subractrCash(unsigned int subtracted_cash);	//Czy got�wka zosta�a odj�ta? (nie zostanie, je�eli jest jej za ma�o)

	void unlockContent(eUnlockedType type, unsigned int ID);
	bool isContentUnlocked(eUnlockedType type, unsigned int ID);

	unsigned int getCash();
	std::string getName();
};

#endif //!profile_h