#include "shopItem.h"

cShopItem::cShopItem()
{
	;
}

cShopItem::cShopItem(sf::String name, sf::String description, eUnlockedType item_type, unsigned short price)
{
	this->name = name;
	this->description = description;
	this->type = item_type;
	this->price = price;
}

cShopItem::cShopItem(eUnlockedType item_type, unsigned int ID)
{
	this->type = item_type;

	switch (item_type)
	{
	case UNLOCKED_MODULATOR:
	{
		switch (ID)
		{
		case 0:
		{
			this->name = L"Little NPCs";
			this->description = L"Size of NPCs is twice reduced.";
			this->price = 50;
			break;
		}
		}
		break;
	}
	}
}


bool cShopItem::viewPurchase(sf::RenderWindow &win)
{
	sf::String type_str = L"";
	switch (this->type)
	{
	case UNLOCKED_MODULATOR: {type_str = L"Modulator"; break;}
	}

	if (yesNoDialog(win, type_str + ": " + this->name, this->description + " Price: " + uIntToStr(this->price), "BUY", "CANCEL"))
		return true;
	return false;
}


sf::String cShopItem::getName()
{
	return this->name;
}

sf::String cShopItem::getDescription()
{
	return this->description;
}

eUnlockedType cShopItem::getType()
{
	return this->type;
}

unsigned short cShopItem::getPrice()
{
	return this->price;
}