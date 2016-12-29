#include "button.h"

cButton::cButton()
{
	;
}

cButton::cButton(sf::Vector2f pos, sf::String str, sf::Texture &texture)
{
	this->setTexture(texture);
	this->setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
	this->setPosition(pos);

	this->setText(str);
}

void cButton::draw(sf::RenderWindow &win)
{
	win.draw(*this);
	win.draw(this->text);
}

bool cButton::isMouseOver(sf::RenderWindow &win)
{
	if (this->getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(win)))
		return true;
	return false;
}

void cButton::changeGraphics(bool is_mouse_over)
{
	if (is_mouse_over)
		this->text.setFillColor(sf::Color(255, 192, 0));
	else
		this->text.setFillColor(sf::Color(64, 64, 64));
}

void cButton::setText(sf::String str)
{
	this->text.setFont(font[1]);
	this->text.setCharacterSize(14);
	this->text.setString(str);
	this->text.setFillColor(sf::Color(64, 64, 64));
	this->text.setOrigin(this->text.getGlobalBounds().width / 2, this->text.getGlobalBounds().height / 2);
	this->text.setPosition(this->getPosition().x, this->getPosition().y);
}

sf::String cButton::getString()
{
	return this->text.getString();
}