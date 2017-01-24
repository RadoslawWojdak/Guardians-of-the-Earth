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
		sModulator modulator;
		modulator.id = ID;

		switch (ID)
		{
		case 0:
		{
			this->name = L"Jumping NPCs";
			this->description = L"All NPCs are jumping.";
			this->price = 50;
			modulator.score_multipler = 0.5f;
			modulator.type = MODULATOR_OBSTRUCTING;
			break;
		}
		case 1:
		{
			this->name = L"NPCs slow-down";
			this->description = L"Speed NPCs x0.5.";
			this->price = 25;
			modulator.score_multipler = -0.2f;
			modulator.type = MODULATOR_FACILIATING;
			break;
		}
		}

		this->features.modulator = modulator;

		break;
	}
	case UNLOCKED_NPC:
	{
		switch (ID)
		{
		case 0:
		{
			this->name = L"NPC 6";
			this->description = L"Unlock new flying NPC.";
			this->price = 50;
			break;
		}
		}

		this->features.id = ID;
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
	case UNLOCKED_NPC: {type_str = L"NPC"; break;}
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

uShopItemFeatures cShopItem::getFeatures()
{
	return this->features;
}