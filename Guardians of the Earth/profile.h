#ifndef profile_h
#define profile_h

#include "SFML/Graphics.hpp"
#include <string>
#include <fstream>

#ifdef _WIN32
    #include <Windows.h>
#endif

#include "dialogs.h"
#include "enums.h"

class cProfile
{
	std::string name;
	unsigned int cash;

	bool **unlocked_content;
	std::vector <std::string> save_name;

public:
	cProfile();

	bool newProfile(sf::RenderWindow &win, std::string name);
	bool saveProfile(sf::RenderWindow &win);
	bool loadProfile(sf::RenderWindow &win, std::string name);

	void addSaveSlot(sf::RenderWindow &win, std::string slot_name);
	void deleteSaveSlot(sf::RenderWindow &win, std::string slot_name);

	void addCash(unsigned int extra_cash);
	bool subractractCash(unsigned int subtracted_cash);	//Czy gotówka zosta³a odjêta? (nie zostanie, je¿eli jest jej za ma³o)

	void unlockContent(eUnlockedType type, unsigned int ID);
	bool isContentUnlocked(eUnlockedType type, unsigned int ID);

	unsigned int getCash();
	std::string getName();
	std::string getSaveSlotName(unsigned int id);
	unsigned int getNumberOfSaveSlots();
};

#endif //!profile_h
