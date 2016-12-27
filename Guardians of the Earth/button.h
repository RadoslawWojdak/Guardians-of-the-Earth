#ifndef button_h
#define button_h

#include "SFML\Graphics.hpp"
#include "graphics.h"
#include "fonts.h"

class cButton :public sf::Sprite
{
	sf::Text text;

public:
	cButton();
	cButton(sf::Vector2f center_pos, sf::String str);
	
	void draw(sf::RenderWindow &win);

	bool cButton::isMouseOver(sf::RenderWindow &win);
	void changeGraphics(bool is_mouse_over);
	
	void setText(sf::String str);
	sf::String getString();
};

#endif //!button_h