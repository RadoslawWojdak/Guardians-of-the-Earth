#include "backgroundobject.h"

cBackgroundObject::cBackgroundObject(eWorld world_type, eBackgroundType type, sf::Vector2f pos)
{
	this->type = type;
	//adjustBgType();

	this->adjustGraphicsParameters(*this->randomGraphics(world_type, pos), pos);

	this->setAllPosition(pos);
}

sf::Texture* cBackgroundObject::randomGraphics(eWorld world_type, sf::Vector2f pos)
{
	this->extra_count = 0;

	switch (this->type)
	{
	case BG_GROUND_LYING:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			int graph = rand() % 2;
			switch (graph)
			{
			case 0: {return &t_background_obj[0][3];	break;}		//Ma³y krzak
			case 1: {this->front = true; return &t_background_obj[0][4];	break;}		//P³ot
			}
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][0];
			break;
		}
		case WORLD_UNDERWATER:
		{
			return &t_background_obj[2][1];	//Muszla
			break;
		}
		case WORLD_ICE_LAND:
		{
			return &t_background_obj[3][4];	//Œnie¿ki
			break;
		}
		case WORLD_DESERT:
		{
			return &t_background_obj[4][4];	//Kaktus
			break;
		}
		}
		break;
		
	}
	case BG_GROUND_LEVITATING:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][3];	//Ma³y krzak (tymczasowo - dopóki nie zrobi³em innych grafik)
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][0];
			break;
		}
		case WORLD_ICE_LAND:
		{
			return &t_background_obj[3][1];	//Kula (góra drzewa)
			break;
		}
		case WORLD_DESERT:
		{
			return &t_background_obj[4][4];	//Kaktus
			break;
		}
		}
		break;
	}
	case BG_TRUNK:
	case BG_TOP_TREE:	//W razie czego doda³em tutaj koronê drzewa, chocia¿ w teorii nie powinna siê nigdy wylosowaæ
	{
		short height = rand() % 4 + 1;		//Wysokoœæ pnia drzewa
		short up_height;					//Wysokoœæ korony drzewa
		
		if (world_type == WORLD_DESERT)		//Pustynia ma grafikê korony drzewa tylko z (32x32)px
			up_height = 1;
		else								//Pozosta³e maj¹ grafiki koron drzew (32x32)px i (32x64)px
			up_height = rand() % 2 + 1;

		this->extra_count = height;			//Wysokoœæ pnia drzewa - 1 + korona drzewa
		
		this->extra_sprite = new cObjectLevel[height];	//Wysokoœæ pnia drzewa - 1 + korona drzewa

														//Tworzenie reszty pnia drzewa
		for (int i = 0; i < height - 1; i++)
		{
			switch (world_type)
			{
			case WORLD_OVERWORLD: {this->extra_sprite[i] = cObjectLevel(t_background_obj[0][0]);	break;}
			case WORLD_ICE_LAND: {this->extra_sprite[i] = cObjectLevel(t_background_obj[3][0]);		break;}
			case WORLD_DESERT: {this->extra_sprite[i] = cObjectLevel(t_background_obj[4][0]);		break;}
			}
			this->extra_sprite[i].setOrigin(this->extra_sprite[i].getTextureRect().width / 2, this->extra_sprite[i].getTextureRect().height / 2);
			this->extra_sprite[i].setPosition(pos.x, pos.y - ((i + 1) * 32));
		}

		//Tworzenie korony drzewa
		if (up_height == 1)
		{
			switch (world_type)
			{
			case WORLD_OVERWORLD: {this->extra_sprite[height - 1] = cObjectLevel(t_background_obj[0][1]);	break;}
			case WORLD_ICE_LAND: {this->extra_sprite[height - 1] = cObjectLevel(t_background_obj[3][1]);	break;}
			case WORLD_DESERT: {this->extra_sprite[height - 1] = cObjectLevel(t_background_obj[4][1]);		break;}
			}
		}
		else
		{
			switch (world_type)
			{
			case WORLD_OVERWORLD: {this->extra_sprite[height - 1] = cObjectLevel(t_background_obj[0][2]);	break;}
			case WORLD_ICE_LAND: {this->extra_sprite[height - 1] = cObjectLevel(t_background_obj[3][2]);	break;}
			}
		}

		this->extra_sprite[height - 1].setOrigin(this->extra_sprite[height - 1].getTextureRect().width / 2, this->extra_sprite[height - 1].getTextureRect().height / 2);
		this->extra_sprite[height - 1].setPosition(pos.x, pos.y - ((height - 1) * up_height * 32));

		switch (world_type)
		{
		case WORLD_OVERWORLD:	return &t_background_obj[0][0];
		case WORLD_ICE_LAND:	return &t_background_obj[3][0];
		case WORLD_DESERT:		return &t_background_obj[4][0];
		}
		break;
	}
	case BG_FLYING:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][5];	//Chmura 64x32
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][5];	//Œwietliki
			break;
		}
		case WORLD_ICE_LAND:
		{
			return &t_background_obj[3][3];	//Chmura 64x32
			break;
		}
		case WORLD_DESERT:
		{
			return &t_background_obj[4][2];	//Chmura 64x32
			break;
		}
		}
		break;
	}
	case BG_GROUND_INSIDE:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][6];	//Zaroœla
			break;
		}
		case WORLD_UNDERGROUND:
		{
			switch (rand() % 3)
			{
			case 0: {return &t_background_obj[1][2];	break;}
			case 1: {return &t_background_obj[1][3];	break;}
			case 2: {return &t_background_obj[1][4];	break;}
			}
			break;
		}
		case WORLD_UNDERWATER:
		{
			return &t_background_obj[2][2];	//Rozgwiazda;
			break;
		}
		case WORLD_ICE_LAND:
		{
			return &t_background_obj[3][5];	//Zaroœla
			break;
		}
		case WORLD_DESERT:
		{
			return &t_background_obj[4][3];	//Czaszka
			break;
		}
		}
		break;
	}
	case BG_FLUID_LYING:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][8];	//Wodorost
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][0];
			break;
		}
		case WORLD_UNDERWATER:
		{
			return &t_background_obj[2][1];	//Muszla
			break;
		}
		}
		break;
	}
	case BG_FLUID_DIVE:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][7];	//b¹bel powietrza
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][0];
			break;
		}
		case WORLD_UNDERWATER:
		{
			switch (rand() % 2)
			{
			case 0:	return &t_background_obj[2][0];	//Ryba
			case 1: return &t_background_obj[2][3];	//B¹bel powietrza
			}
			break;
		}
		}
		break;
	}
	case BG_FLUID_FLOAT:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][9];	//kaczka
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][0];
			break;
		}
		case WORLD_UNDERWATER:
		{
			return &t_background_obj[2][0];	//Ryba
			break;
		}
		}
		break;
	}
	case BG_CEILING:
	{
		switch (world_type)
		{
		case WORLD_OVERWORLD:
		{
			return &t_background_obj[0][5];	//chmura
			break;
		}
		case WORLD_UNDERGROUND:
		{
			return &t_background_obj[1][1];
			break;
		}
		case WORLD_ICE_LAND:
		{
			return &t_background_obj[3][3];	//Chmura
			break;
		}
		}
		break;
	}
	}

	return &t_background_obj[0][0];
}

void cBackgroundObject::setAllPosition(sf::Vector2f pos)
{
	if (this->type == BG_GROUND_LYING || this->type == BG_FLUID_LYING)
		this->setPosition(sf::Vector2f(pos.x, pos.y + (16 - this->getTextureRect().height / 2)));	//Obiekty w tle s¹ przyci¹gane do spodu siatki 32x32
	else if (this->type == BG_FLUID_FLOAT)	//Czêœæ dopasowywania grafiki jest ustawiona przy tworzeniu obiektu w "map.cpp"
		this->setPosition(sf::Vector2f(pos.x, pos.y - (16 - this->getTextureRect().height / 2)));	//Obiekty p³ywaj¹ce na wodzie s¹ wzniesione na górê siatki 32x32
	else
		this->setPosition(sf::Vector2f(pos.x, pos.y));	//Obiekty znajduj¹ce siê w ziemi i p³ywaj¹ce w wodzie nie s¹ przyci¹gane do spodu siatki

	int height = this->getTextureRect().height;	//Aktualna wysokoœæ ca³ej grafiki
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

bool cBackgroundObject::isGroundCollision(bool *ground_exists, sf::Vector2i grid_size)
{
	sf::Vector2i pos_on_grid = this->posOnGrid(sf::Vector2i(32, 32));	//Pozycja obiektu na siatce (w którym kwadracie siatki znajduje siê obiekt)
	
	//Czy wysz³o poza zakres? (poza ramy poziomu)
	if (pos_on_grid.x < 0 || pos_on_grid.x >= grid_size.x || pos_on_grid.y < 0 || pos_on_grid.y >= grid_size.y)
		return false;
	//Sprawdzanie g³ównej czêœci grafiki
	if (ground_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
		return true;
	//Sprawdzanie grafik dodatkowych
	for (unsigned short i = 0; i < this->extra_count; i++)
	{
		pos_on_grid = this->extra_sprite[i].posOnGrid(sf::Vector2i(32, 32));
		if (ground_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
			return true;
	}

	return false;
}

bool cBackgroundObject::isWaterCollision(bool *water_exists, sf::Vector2i grid_size)
{
	sf::Vector2i pos_on_grid = this->posOnGrid(sf::Vector2i(32, 32));	//Pozycja obiektu na siatce (w którym kwadracie siatki znajduje siê obiekt)
	
	//Czy wysz³o poza zakres? (poza ramy poziomu)
	if (pos_on_grid.x < 0 || pos_on_grid.x >= grid_size.x || pos_on_grid.y < 0 || pos_on_grid.y >= grid_size.y)
		return false;
	//Sprawdzanie g³ównej czêœci grafiki
	if (water_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
		return true;
	//Sprawdzanie grafik dodatkowych
	for (unsigned short i = 0; i < this->extra_count; i++)
	{
		pos_on_grid = this->extra_sprite[i].posOnGrid(sf::Vector2i(32, 32));
		if (water_exists[pos_on_grid.y * grid_size.x + pos_on_grid.x])
			return true;
	}
	
	return false;
}