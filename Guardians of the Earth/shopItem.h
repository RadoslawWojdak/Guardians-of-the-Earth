#ifndef shopItem_h
#define shopItem_h

#include "SFML\Graphics.hpp"
#include "enums.h"
#include "dialogs.h"
#include "actionOnText.h"

class cShopItem
{
	sf::String name, description;
	eUnlockedType type;
	unsigned short price;

public:
	cShopItem();
	cShopItem(sf::String name, sf::String description, eUnlockedType item_type, unsigned short price);
	cShopItem(eUnlockedType item_type, unsigned int ID);

	bool viewPurchase(sf::RenderWindow &win);

	sf::String getName();
	sf::String getDescription();
	eUnlockedType getType();
	unsigned short getPrice();
};

#endif //!shopItem_h