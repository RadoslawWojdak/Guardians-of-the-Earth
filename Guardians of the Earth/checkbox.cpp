#include "checkbox.h"

cCheckbox::cCheckbox()
{
	;
}

cCheckbox::cCheckbox(sf::Vector2f left_top_corner, sf::String text, sf::Color &text_color, bool is_checked)
{
	this->check = is_checked;

	//Dzia³ania na tekœcie
	this->text.setString(text);
	this->text.setFont(font[1]);
	this->text.setCharacterSize(12);
	this->text.setFillColor(text_color);
	this->text.setPosition(left_top_corner.x + t_checkbox.getSize().x + 4, left_top_corner.y);

	//Dzia³ania na grafice checkbox
	this->setTexture(t_checkbox);
	this->setTextureRect(sf::IntRect(0, is_checked ? t_checkbox.getSize().y / 2 : 0, t_checkbox.getSize().x, t_checkbox.getSize().y / 2));
	this->setPosition(left_top_corner);
}

void cCheckbox::clicked()
{
	if (this->check)
		this->check = false;
	else
		this->check = true;

	this->setTextureRect(sf::IntRect(0, this->check ? t_checkbox.getSize().y / 2 : 0, t_checkbox.getSize().x, t_checkbox.getSize().y / 2));
}

void cCheckbox::draw(sf::RenderWindow &win)
{
	win.draw(*this);
	win.draw(this->text);
}

void cCheckbox::changeTextColor(sf::Color &color)
{
	this->text.setFillColor(color);
}

bool cCheckbox::isChecked()
{
	return this->check;
}

bool cCheckbox::isMouseOver(sf::RenderWindow &win)
{
	sf::FloatRect checkbox_field;
	checkbox_field.left = this->getGlobalBounds().left;
	checkbox_field.top = this->getGlobalBounds().top > this->text.getGlobalBounds().top ? this->getGlobalBounds().top : this->text.getGlobalBounds().top;
	checkbox_field.width = (this->text.getGlobalBounds().left + this->text.getGlobalBounds().width) - this->getGlobalBounds().left;
	checkbox_field.height = this->getGlobalBounds().height > this->text.getGlobalBounds().height ? this->getGlobalBounds().height : this->text.getGlobalBounds().height;

	if (checkbox_field.contains((sf::Vector2f)sf::Mouse::getPosition(win)))
		return true;
	return false;
}