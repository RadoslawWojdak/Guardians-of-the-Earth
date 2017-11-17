#ifndef button_h
#define button_h

#include "SFML/Graphics.hpp"
#include "graphics.h"
#include "fonts.h"

class cButton :public sf::Sprite
{
protected:
	sf::Text text;
	sf::Color text_color;

public:
	cButton();
	cButton(sf::Vector2f center_pos, sf::String str, sf::Texture &texture = t_button, sf::Color &text_color = sf::Color(64, 64, 64));

	void draw(sf::RenderWindow &win);

	bool cButton::isMouseOver(sf::RenderWindow &win);
	void changeGraphics(bool is_mouse_over, sf::Color &color);

	void setText(sf::String str);
	sf::String getString();
	void setPosition(sf::Vector2f &position);
	void setPosition(float x, float y);
};

#endif //!button_h
