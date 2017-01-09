#ifndef shop_h
#define shop_h

#include "SFML\Graphics.hpp"
#include <vector>
#include <sstream>
#include "character.h"
#include "graphics.h"
#include "fonts.h"
#include "button.h"

class cItemShop :public cButton
{
	bool discount;
	unsigned int value;

public:
	cItemShop();
	cItemShop(sf::Vector2f center_pos, unsigned int value, sf::Texture &texture, bool discount = false);

	void setValue(unsigned int value);
	unsigned int getValue();
	bool isDiscount();
	void setPosition(sf::Vector2f &position);
	void setPosition(float x, float y);
};

class cShop
{
	std::vector <cCharacter*> player;
	
	std::vector <cItemShop> item;
	cItemShop bonus[2];
	cItemShop rebirth;
	cItemShop extra_life;
	cItemShop pet_hp;
	cButton exit_button;
	
public:
	cShop(std::vector <cCharacter> &player, bool *modulators);

	bool shopMenu(sf::RenderWindow &win);	//Zwraca wartoœæ FALSE, gdy okno programu zosta³o zamkniête
};

#endif //!shop_h