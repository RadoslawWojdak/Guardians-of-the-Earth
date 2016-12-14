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
	
	this->setOrigin(this->getTextureRect().width / 2, this->getTextureRect().height / 2);
	this->setPosition(pos);
}

//UWAGA!!! Je¿eli coœ tutaj zmienisz, nie zapomnij zmieniæ równie¿ w map.cpp przy pêtli spawn_pu_pos, która korzysta z takiego samego algorytmu!
void cObjectLevel::adjustObjectToLevel(unsigned int map_height)
{
	this->setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y + map_height - g_height));
}

sf::Vector2i cObjectLevel::posOnGrid(sf::Vector2i grid_rect_size)
{
	sf::Vector2i pos;

	pos.x = (this->getPosition().x) / 32;
	pos.y = (this->getPosition().y - 32) / 32;

	return pos;
}

bool cObjectLevel::isCollisionOnGrid(bool *object_on_grid_exists, sf::Vector2i grid_size)
{
	sf::Vector2i pos_on_grid = this->posOnGrid(sf::Vector2i(32, 32));

	//Czy wysz³o poza zakres? (poza ramy poziomu)
	if (pos_on_grid.x < 0 || pos_on_grid.x >= grid_size.x || pos_on_grid.y < 0 || pos_on_grid.y >= grid_size.y)
		return false;
	
	if (object_on_grid_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
		return true;

	return false;
}