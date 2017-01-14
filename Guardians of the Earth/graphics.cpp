#include "graphics.h"

sf::Texture t_block[2];
sf::Texture t_bonus_block[2];
sf::Texture t_gold_bonus_block[2];
sf::Texture t_ground[5][47];
sf::Texture t_treasure[8];
sf::Texture t_power_up[2];
sf::Texture t_background[5];
sf::Texture *t_background_obj[5];
sf::Texture t_npc[5];
sf::Texture t_object[8];
sf::Texture t_character[2];
sf::Texture t_characters_bonus[2][2];
sf::Texture t_characters_bonus_icon[2][2];
sf::Texture t_characters_skill[2][4];
sf::Sprite s_characters_skill[2][4];
sf::Texture t_pet[2];

sf::Texture t_stats_window;
sf::Texture t_pause_menu_window;
sf::Texture t_dialog_window;
sf::Texture t_heart;
sf::Texture t_experience_bar;
sf::Texture t_discount_sign;
sf::Texture t_button;
sf::Texture t_checkbox;
sf::Texture t_profile_button;
sf::Texture t_button_rebirth;
sf::Texture t_button_extra_life;
sf::Texture t_button_extra_hp;
sf::Texture t_close_button;

bool initGraph()
{
	//BLOKI
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i+1;
		nr = ss.str();
		ss.clear();
		
		std::string path = "graphics\\blocks\\block-" + nr + ".png";
		if (!t_block[i].loadFromFile(path))
			return false;
	}
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\blocks\\bonus_block-" + nr + ".png";
		if (!t_bonus_block[i].loadFromFile(path))
			return false;
	}
	for (unsigned int i = 0; i < 1; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\blocks\\gold_bonus_block-" + nr + ".png";
		if (!t_gold_bonus_block[i].loadFromFile(path))
			return false;
	}

	//ZIEMIA
	for (unsigned int i = 0; i < 5; i++)
	{
		std::string world;
		switch (i)
		{
		case 0:	{world = "overworld";	break;}
		case 1: {world = "underground";	break;}
		case 2: {world = "underwater";	break;}
		case 3: {world = "ice_land";	break;}
		case 4: {world = "desert";		break;}
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
	for (unsigned int i = 0; i < 8; i++)
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

	//power upy
	for (unsigned int i = 0; i < 2; i++)
	{
		std::string nr;
		std::stringstream ss;
		ss << i + 1;
		nr = ss.str();
		ss.clear();

		std::string path = "graphics\\bonuses\\power_up-" + nr + ".png";
		if (!t_power_up[i].loadFromFile(path))
			return false;
	}


	//TLO
	for (unsigned int i = 0; i < 5; i++)
	{
		std::string world;
		switch (i)
		{
		case 0: {world = "overworld";	break;}
		case 1: {world = "underground";	break;}
		case 2: {world = "underwater";	break;}
		case 3: {world = "ice_land";	break;}
		case 4: {world = "desert";		break;}
		}

		/*for (int j = 0; j < 1; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\backgrounds\\" + world + "\\background-" + nr + ".png";
			if (!t_background[i][j].loadFromFile(path))
				return false;
		}*/
		std::string path = "graphics\\backgrounds\\" + world + "\\background-1.png";
		if (!t_background[i].loadFromFile(path))
			return false;
	}

	//OBIEKTY W TLE
	for (unsigned int i = 0; i < 5; i++)
	{
		std::string world;
		int how_many;	//Jak wiele jest grafik w danym typie poziomu (ka¿dy typ mo¿e mieæ ró¿n¹ iloœæ grafik)
		switch (i)
		{
		case 0: {world = "overworld";	how_many = 10;	break;}
		case 1: {world = "underground";	how_many = 6;	break;}
		case 2: {world = "underwater";	how_many = 4;	break;}
		case 3: {world = "ice_land";	how_many = 6;	break;}
		case 4: {world = "desert";		how_many = 5;	break;}
		}

		t_background_obj[i] = new sf::Texture[how_many];

		for (int j = 0; j < how_many; j++)
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
	for (unsigned int i = 0; i < 5; i++)
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
	for (unsigned int i = 0; i < 8; i++)
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

	//POSTACIE
	//Rycerz
	for (unsigned int i = 0; i < 2; i++)
	{
		std::string character = "";
		switch (i)
		{
		case 0: {character = "knight"; break;}
		case 1: {character = "archer"; break;}
		}

		std::string path = "graphics\\characters\\" + character + "\\" + character + "-1.png";
		if (!t_character[i].loadFromFile(path))
			return false;
		for (int j = 0; j < 1; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\characters\\" + character + "\\bonus-" + nr + ".png";
			if (!t_characters_bonus[i][j].loadFromFile(path))
				return false;
		}
		for (int j = 0; j < 2; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\characters\\" + character + "\\bonus_icon-" + nr + ".png";
			if (!t_characters_bonus_icon[i][j].loadFromFile(path))
				return false;
		}
		for (int j = 0; j < 4; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();
			ss.clear();

			std::string path = "graphics\\characters\\" + character + "\\skill-" + nr + ".png";
			if (!t_characters_skill[i][j].loadFromFile(path))
				return false;
			s_characters_skill[i][j].setTexture(t_characters_skill[i][j]);
		}
		//PET-Y
		for (int j = 0; j < 2; j++)
		{
			std::string path = "graphics\\characters\\" + character + "\\pet.png";
			if (!t_pet[i].loadFromFile(path))
				return false;
		}
	}

	//INNE
	if (!t_stats_window.loadFromFile("graphics\\others\\stats_window.png"))
		return false;
	if (!t_pause_menu_window.loadFromFile("graphics\\others\\pause_menu_window.png"))
		return false;
	if (!t_dialog_window.loadFromFile("graphics\\others\\dialog_window.png"))
		return false;
	if (!t_heart.loadFromFile("graphics\\others\\heart.png"))
		return false;
	if (!t_experience_bar.loadFromFile("graphics\\others\\experience_bar.png"))
		return false;
	if (!t_discount_sign.loadFromFile("graphics\\others\\discount_sign.png"))
		return false;
	if (!t_button.loadFromFile("graphics\\others\\button-1.png"))
		return false;
	if (!t_checkbox.loadFromFile("graphics\\others\\checkbox.png"))
		return false;
	if (!t_profile_button.loadFromFile("graphics\\others\\profile_button.png"))
		return false;
	if (!t_button_rebirth.loadFromFile("graphics\\others\\button_rebirth.png"))
		return false;
	if (!t_button_extra_life.loadFromFile("graphics\\others\\button_extra_life.png"))
		return false;
	if (!t_button_extra_hp.loadFromFile("graphics\\others\\button_extra_hp.png"))
		return false;
	if (!t_close_button.loadFromFile("graphics\\others\\close_button.png"))
		return false;

	return true;
}


bool shutdownGraph()
{
	return true;
}