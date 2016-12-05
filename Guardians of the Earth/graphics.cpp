#include "graphics.h"

sf::Texture t_block_overworld[2];
sf::Texture t_bonus_block_overworld[2];
sf::Texture t_ground[2][47];
sf::Texture t_treasure[7];
sf::Texture t_background[2][2];
sf::Texture t_background_obj[2][9];
sf::Texture t_npc[2];
sf::Texture t_object[3];

bool init_graph()
{
	//BLOKI
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i+1;
		nr = ss.str();
		ss.clear();
		
		std::string path = "graphics\\blocks\\\overworld\\block-" + nr + ".png";
		if (!t_block_overworld[i].loadFromFile(path))
			return false;
	}
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\blocks\\\overworld\\bonus_block-" + nr + ".png";
		if (!t_bonus_block_overworld[i].loadFromFile(path))
			return false;
	}

	//ZIEMIA
	for (unsigned int i = 0; i < 2; i++)
	{
		std::string world;
		switch (i)
		{
		case 0:	world = "overworld";	break;
		case 1: world = "underground";	break;
		}

		std::string nr = "";
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.str("");

		for (unsigned int j = 0; j < 47; j++)
		{
			std::string nr2 = "";
			ss << j + 1;
			nr2 = ss.str();
			ss.str("");

			std::string path = "graphics\\grounds\\" + world + "\\ground-1-" + nr2 + ".png";
			if (!t_ground[i][j].loadFromFile(path))
				return false;
		}
	}

	//BONUSY
	//Skarby
	for (unsigned int i = 0; i < 7; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\bonuses\\treasure-" + nr + ".png";
		if (!t_treasure[i].loadFromFile(path))
			return false;
	}

	//TLO
	for (unsigned int i = 0; i < 2; i++)
	{
		std::string world;
		switch (i)
		{
		case 0:	world = "overworld";	break;
		case 1: world = "underground";	break;
		}

		for (int j = 0; j < 1; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\backgrounds\\" + world + "\\background-" + nr + ".png";
			if (!t_background[i][j].loadFromFile(path))
				return false;
		}
	}

	//OBIEKTY W TLE
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string world;
		switch (i)
		{
		case 0:	world = "overworld";	break;
		case 1: world = "underground";	break;
		}

		for (int j = 0; j < 9; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\background_objects\\" + world + "\\background-" + nr + ".png";
			if (!t_background_obj[i][j].loadFromFile(path))
				return false;
		}
	}

	//NPC-Y
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\npcs\\npc-" + nr + ".png";
		if (!t_npc[i].loadFromFile(path))
			return false;
	}

	//OBIEKTY POZIOMU
	for (unsigned int i = 0; i < 3; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\level_objects\\object-" + nr + ".png";
		if (!t_object[i].loadFromFile(path))
			return false;
	}

	return true;
}