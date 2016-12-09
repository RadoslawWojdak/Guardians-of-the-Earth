#include "objectlevel.h"

cObjectLevel::cObjectLevel()
{
	;
}

cObjectLevel::cObjectLevel(sf::Texture &texture) 
	: sf::Sprite(texture)
{
	;
}

void cObjectLevel::adjustGraphicsParameters(sf::Texture &texture, sf::Vector2f pos)
{
	this->setTexture(texture);
	
	//Przycinanie tekstury do maksymalnego rozmiaru 32x32 (jedno pole mapy)
	/*int new_width = this->getTextureRect().width, new_height = this->getTextureRect().height;
	if (this->getTextureRect().width > 32)
		new_width = 32;
	if (this->getTextureRect().height > 32)
		new_height = 32;
	this->setTextureRect(sf::IntRect(0, 0, new_width, new_height));
	*/
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}

//UWAGA!!! Je¿eli coœ tutaj zmienisz, nie zapomnij zmieniæ równie¿ w map.cpp przy pêtli spawn_pu_pos!
void cObjectLevel::adjustObjectToLevel(unsigned int map_height)
{
	this->setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y + map_height - g_height));
}
/*
bool cObjectLevel::isInView(sf::View &view)
{
	if (this->getLocalBounds().intersects(view.getViewport()))
		return true;
	return false;
}*/

sf::Vector2i cObjectLevel::posOnGrid(sf::Vector2i grid_rect_size)
{
	sf::Vector2i pos;

	pos.x = (this->getPosition().x) / 32;// +(this->getLocalBounds().width > 32 ? 1 : 0);
	pos.y = (this->getPosition().y - 32) / 32;// +(this->getLocalBounds().height > 32 ? 1 : 0);

	return pos;
}