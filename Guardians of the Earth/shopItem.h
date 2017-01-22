#ifndef shopItem_h
#define shopItem_h

#include "SFML\Graphics.hpp"
#include "enums.h"
#include "dialogs.h"
#include "actionOnText.h"

struct sModulator
{
	eModulatorType type;
	float score_multipler;
	int id;
};

union uShopItemFeatures
{
	sModulator modulator;
	int id;
};

class cShopItem
{
	uShopItemFeatures features;

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
	uShopItemFeatures getFeatures();
};

#endif //!shopItem_h