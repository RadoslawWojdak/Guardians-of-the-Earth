#include "backgroundobject.h"

cBackgroundObject::cBackgroundObject(eType type, sf::Vector2f pos)
{
	this->type = type;

	this->adjustGraphicsParameters(*this->randomGraphics(pos), pos);

	this->setAllPosition(pos);
}

sf::Texture* cBackgroundObject::randomGraphics(sf::Vector2f pos)
{
	this->extra_count = 0;

	switch (this->type)
	{
	case TYPE_GROUND:
	{
		int graph = rand() % 3;
		switch (graph)
		{
		case 0:	//Pie� drzewa - tworzenie drzew
		{
			short height = rand() % 4 + 1;		//Wysoko�� pnia drzewa
			short up_height = rand() % 2 + 1;	//Wysoko�� korony drzewa
			this->extra_count = height;			//Wysoko�� pnia drzewa - 1 + korona drzewa

			this->extra_sprite = new sf::Sprite[height];	//Wysoko�� pnia drzewa - 1 + korona drzewa

			//Tworzenie reszty pnia drzewa
			for (int i = 0; i < height - 1; i++)
			{
				this->extra_sprite[i] = sf::Sprite(t_background_obj[0][0]);
				this->extra_sprite[i].setOrigin(this->extra_sprite[i].getTextureRect().width / 2, this->extra_sprite[i].getTextureRect().height / 2);
				this->extra_sprite[i].setPosition(pos.x, pos.y - ((i + 1) * 32));
			}

			//Tworzenie korony drzewa
			if (up_height == 1)
				this->extra_sprite[height - 1] = sf::Sprite(t_background_obj[0][1]);
			else
				this->extra_sprite[height - 1] = sf::Sprite(t_background_obj[0][2]);

			this->extra_sprite[height - 1].setOrigin(this->extra_sprite[height - 1].getTextureRect().width / 2, this->extra_sprite[height - 1].getTextureRect().height / 2);
			this->extra_sprite[height - 1].setPosition(pos.x, pos.y - ((height - 1) * up_height * 32));

			return &t_background_obj[0][0];
			break;
		}
		//Pojedy�cze obiekty (np. p�oty i ma�e krzaki)
		case 1: return &t_background_obj[0][3];		break;		//Ma�y krzak
		case 2: return &t_background_obj[0][4];		break;		//P�ot
		}
		break;
	}
	case TYPE_AIR:
	{
		return &t_background_obj[0][5];
		break;
	}
	case TYPE_WATER:	//TODO stworzy� wodne tekstury
	{
		return &t_background_obj[0][7];
		break;
	}
	case TYPE_SOIL:
	{
		return &t_background_obj[0][6];
		break;
	}
	case TYPE_GROUND_WATER:
	{
		return &t_background_obj[0][8];
		break;
	}
	}

	return &t_background_obj[0][0];
}

void cBackgroundObject::setAllPosition(sf::Vector2f pos)
{
	if (this->type != TYPE_SOIL && this->type != TYPE_WATER)
		this->setPosition(sf::Vector2f(pos.x, pos.y + (16 - this->getTextureRect().height / 2)));	//Obiekty w tle s� przyci�gane do spodu siatki 32x32
	else
		this->setPosition(sf::Vector2f(pos.x, pos.y));	//Obiekty znajduj�ce si� w ziemi i p�ywaj�ce w wodzie nie s� przyci�gane do spodu siatki

	int height = this->getTextureRect().height;	//Aktualna wysoko�� ca�ej grafiki
	for (int i = 0; i < this->extra_count; i++)
	{
		this->extra_sprite[i].setPosition(sf::Vector2f(pos.x, pos.y - height));
		height += this->extra_sprite[i].getTextureRect().height;
	}
}

void cBackgroundObject::drawAllGraphics(sf::RenderWindow &win)
{
	win.draw(*this);
	for (short i = 0; i < this->extra_count; i++)
		win.draw(this->extra_sprite[i]);
}

bool cBackgroundObject::isGroundCollision(std::vector <cGround> &ground)
{
	for (unsigned int i = 0; i < ground.size(); i++)
	{
		//Kolizja z g��wn� grafik�
		if (ground[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
		//Kolizja z dodatkowymi grafikami
		for (short j = 0; j < this->extra_count; j++)
			if (ground[i].getGlobalBounds().intersects(this->extra_sprite[j].getGlobalBounds()))
				return true;
	}

	return false;
}

bool cBackgroundObject::isWaterCollision(std::vector <cWater> &water)
{
	for (unsigned int i = 0; i < water.size(); i++)
	{
		//Kolizja z g��wn� grafik�
		if (water[i].getGlobalBounds().intersects(this->getGlobalBounds()))
			return true;
		//Kolizja z dodatkowymi grafikami
		for (short j = 0; j < this->extra_count; j++)
			if (water[i].getGlobalBounds().intersects(this->extra_sprite[j].getGlobalBounds()))
				return true;
	}

	return false;
}