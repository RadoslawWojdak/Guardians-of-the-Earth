#ifndef checkbox_h
#define checkbox_h

#include <SFML/Graphics.hpp>
#include "graphics.h"
#include "fonts.h"

class cCheckbox :private sf::Sprite
{
	bool check;
	sf::Text text;

public:
	cCheckbox();
	cCheckbox(sf::Vector2f left_top_corner, sf::String text, sf::Color text_color = sf::Color(255, 255, 255), bool is_checked = false); //sf::Color &text_color = sf::Color(255, 255, 255)

	void clicked();
	void draw(sf::RenderWindow &win);

	void changeTextColor(sf::Color &color);

	bool isChecked();
	bool isMouseOver(sf::RenderWindow &win);
};

#endif //!checkbox_h
