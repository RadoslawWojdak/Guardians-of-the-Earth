#include "map.h"

cMap::cMap(sf::RenderWindow &win, cProfile &profile, eWorld world, short &number_of_players, eCharacter character[], bool *modulators, std::string &new_slot, std::string &loaded_slot)
	:physics_world(b2Vec2(0.0f, 10.0f))
{
	this->level_number = 1;
	this->world_type = world;
	this->prev_sector.clear();

	if (loaded_slot != "")
	{
		this->loadGame(win, profile, loaded_slot, modulators);
		number_of_players = this->player_number;
		this->levelGenerator(win, profile, loaded_slot, this->player_number, modulators, false, false, character);
	}
	else
		this->levelGenerator(win, profile, new_slot, number_of_players, modulators, false, false, character);
}

void cMap::levelGenerator(sf::RenderWindow &win, cProfile &profile, std::string &slot_name, short number_of_players, bool *modulators, bool refresh, bool next_level, eCharacter character[])
{
	this->experience_countdown = (180 + 3.5f * (this->level_number - 1)) * g_framerate_limit;
	this->player_number = number_of_players;
	this->golden_bb_created = false;

	if (refresh || next_level)
		this->destroy(false);
	if (next_level)
	{
		this->level_number++;
		this->world_type = (eWorld)(rand() % 5);
	}
	if (!refresh)
		this->reserve_sector.clear();

	groundsInit(this->physics_world);
	fluidInit(this->physics_world);

	this->width = 0;
	this->height = 600;

	this->x_generate = 0;

	//GENERATOR POZIOMU (TEREN)
	//Przypisanie tekstury do t�a (s� 2 - jedno t�o le�y za drugim (dzi�ki temu t�o mo�e si� przesuwa�))
	for (int i = 0; i < 2; i++)
	{
		background[i].setTextureRect(sf::IntRect(0, 0, t_background[this->world_type].getSize().x, t_background[this->world_type].getSize().y));
		background[i].setTexture(t_background[this->world_type]);
	}

	//Wektor zmiennych odpowiedzialnych za pozycje spawn�w bonus�w
	std::vector <sf::Vector2f> spawn_pu_pos;

	//Utworzenie zmiennej sektor
	cSector sector;

	//Tworzenie p�askiego sektora przed map�
	for (unsigned short i = 0; i < 16; i++)
		ground.push_back(cGround(sf::Vector2f(x_generate + i * 32 + 16, this->height - 16), this->world_type));

	x_generate += 512;
	this->width = x_generate;

	this->prev_sector.clear();

	//P�tla tworzenia terenu
	for (int i = 0; i < 50 + (this->level_number - 1); i++)	//Ilo�� sektor�w znajduj�cych si� na mapie
	{
		if (!refresh)	//Tworzenie poziomu od podstaw
		{
			//P�tla wyszukiwania pasuj�cego sektora
			do
			{
				sector.loadRandomSector(win, this->world_type);
			} while (!sector.isSectorFitted(this->world_type, this->prev_sector, this->getHeight()));

			reserve_sector.push_back(sector);

			//Przypisanie sektora poprzedniemu sektorowi - dzi�ki temu mo�na b�dzie znowu wyszukiwa� pasuj�cy sektor
			this->prev_sector = sector;
		}
		else	//Skorzystanie z gotowego poziomu (sektory)
			sector = this->reserve_sector[i];

		//Dla �wiata podziemnego generator tworzy ponad sektorem warstwy gruntu
		if (this->world_type == WORLD_UNDERGROUND)
		{
			for (unsigned int i = 0; i < ceil((float)this->height / 32) - sector.getHeight(); i++)
				for (unsigned int j = 0; j < sector.getWidth(); j++)
				{
					this->ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 - (32 - this->height % 32)), this->world_type));
				}
		}
		//!Dla �wiata podziemnego generator tworzy ponad sektorem warstwy gruntu
		/*Poziomy podwodne nie s� wype�niane wod� gdy� by�oby to zbyt zasobo�erne i w dodatku nie mo�na by by�o doda� t�a.*/

		//Je�eli sektor jest wy�szy od aktualnej wysoko�ci poziomu, to ca�y poziom staje si� wy�szy (kamera mo�e pod��a� wy�ej)
		if (sector.getHeight() * 32 + 64 > this->height)
			this->height = sector.getHeight() * 32 + 64;

		int to_down = g_height - sector.getHeight() * 32;	//Przyrownanie calego terenu do podloza mapy (wysokosc mapy - wysokosc sektora)
		for (int i = 0; i < sector.getHeight(); i++)
			for (int j = 0; j < sector.getWidth(); j++)
			{
				switch (sector.getObject(j, i))
				{
				case eObjType::OBJECT_GROUND: {ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), this->world_type)); break;}
				case eObjType::OBJECT_BLOCK: {block.push_back(cBlock(this->physics_world, t_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				case eObjType::OBJECT_BONUS_BLOCK: {bonus_block.push_back(cBonusBlock(this->physics_world, t_bonus_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				case eObjType::OBJECT_FLUID:
				{
					if (this->world_type != WORLD_UNDERWATER)	//�wiat podwodny jest ju� wype�niony wod�, a obiekty wody tylko by niepotrzebnie spowalnia�y program (przy okazji teraz mo�na doda� t�o)
						fluid.push_back(cFluid(this->world_type, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));
					break;
				}
				case eObjType::OBJECT_TREASURE: {treasure.push_back(cTreasure(this->physics_world, this->world_type, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_TRAMPOLINE: {trampoline.push_back(cTrampoline(this->physics_world, 1, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), 5.0f));	break;}
				case eObjType::OBJECT_POWER_UP: {spawn_pu_pos.push_back(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down));	break;}
				case eObjType::OBJECT_LADDER: {ladder.push_back(cLadder(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				}
			}

		//Zwi�kszanie szeroko�ci poziomu
		x_generate += sector.getWidth() * 32;
		this->width = x_generate;
	}

	//Tworzenie p�askiego sektora za map�
	for (unsigned short i = 0; i < 16; i++)
		ground.push_back(cGround(sf::Vector2f(x_generate + i * 32 + 16, this->height - 16), this->world_type));

	x_generate += 512;
	this->width = x_generate;


	//Dostosowywanie obiekt�w mapy do ustawie� mapy
	for (unsigned int i = 0; i < ground.size(); i++)
		this->ground[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < fluid.size(); i++)
		this->fluid[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < block.size(); i++)
		this->block[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < bonus_block.size(); i++)
		this->bonus_block[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < treasure.size(); i++)
		this->treasure[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < trampoline.size(); i++)
		this->trampoline[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < ladder.size(); i++)
		this->ladder[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < spawn_pu_pos.size(); i++)		//Ta sama zasada dzia�ania, co w przypadku poprzednich p�tli
		spawn_pu_pos[i] = sf::Vector2f(spawn_pu_pos[i].x, spawn_pu_pos[i].y + this->height - g_height);
	//!Dostosowywanie obiekt�w mapy do ustawie� mapy


	//Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu
	sf::Vector2i grid_size(this->width / 32, this->height / 32);	//Wymiary siatki (wymiary poziomu podzielone przez 32)
	bool *is_solid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� sztywny obiekt (grunt, blok, ...) - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_ground = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� grunt - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_fluid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� p�yn - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *to_fluid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� obiekt kt�ry ma wp�yw na wygl�d p�ynu - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_npc = new bool[grid_size.x * grid_size.y];		//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� npc - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie

	//Wymagana oddzielna tablica dynamiczna dla wody w rozgryce, gdy� tablica ta jest wi�ksza od pozosta�ych (o 1 w ka�d� stron�)
	this->fluid_tab = new bool[(grid_size.x + 1) * (grid_size.y + 1)];	//Tablica odpowiedzialna za optymalizacj� rozgrywki pod wzgl�dem p�yn�w; +1 - mapa nie zawsze jest d�ugo�ci podzielnej przez 32

	for (unsigned int i = 0; i < grid_size.y; i++)
		for (unsigned int j = 0; j < grid_size.x; j++)
		{
			is_solid[i * grid_size.x + j] = false;
			is_ground[i * grid_size.x + j] = false;
			to_fluid[i * grid_size.x + j] = false;
			is_npc[i * grid_size.x + j] = false;

			if (this->world_type == WORLD_UNDERWATER)		//W typie podwodnym wsz�dzie jest woda
				is_fluid[i * grid_size.x + j] = true;
			else
				is_fluid[i * grid_size.x + j] = false;
		}
	//Dla p�yn�w jest o 1 pole w ka�d� stron� wi�cej
	for (unsigned int i = 0; i <= grid_size.y; i++)
		for (unsigned int j = 0; j <= grid_size.x; j++)
		{
			if (this->world_type == WORLD_UNDERWATER)		//W typie podwodnym wsz�dzie jest woda
				this->fluid_tab[i * grid_size.x + j] = true;
			else
				this->fluid_tab[i * grid_size.x + j] = false;
		}

	for (unsigned int i = 0; i < this->ground.size(); i++)
	{
		sf::Vector2i pos = this->ground[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		is_ground[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W �wiecie underworld wszystko jest wod� (trzeba wy��czy� pewne elementy, �eby nie pojawia�y si� na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->block.size(); i++)
	{
		sf::Vector2i pos = this->block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W �wiecie underworld wszystko jest wod� (trzeba wy��czy� pewne elementy, �eby nie pojawia�y si� na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
	{
		sf::Vector2i pos = this->bonus_block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W �wiecie underworld wszystko jest wod� (trzeba wy��czy� pewne elementy, �eby nie pojawia�y si� na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->fluid.size(); i++)
	{
		sf::Vector2i pos = this->fluid[i].posOnGrid(sf::Vector2i(32, 32));
		this->fluid_tab[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = true;

		//W "Ice land" p�yn jest traktowany jako solidny obiekt
		if (this->world_type == WORLD_ICE_LAND)
			is_solid[pos.y * grid_size.x + pos.x] = true;
		//Dodatkowo je�eli ostatnim elementem na osi X jest p�yn, to nast�pnym te� jest p�yn (naprawa b��d�w dotycz�cych ostatnich p�yn�w w przypadku gdy szeroko�� poziomu nie jest podzielna przez 32)
		if (pos.x == grid_size.x - 1)
			this->fluid_tab[pos.y * grid_size.x + pos.x + 1] = true;
		//Dodatkowo je�eli ostatnim elementem na osi Y jest p�yn, to nast�pnym te� jest p�yn (naprawa b��d�w dotycz�cych ostatnich p�yn�w w przypadku gdy wysoko�� poziomu nie jest podzielna przez 32)
		if (pos.y == grid_size.y - 1)
			this->fluid_tab[(pos.y + 1) * grid_size.x + pos.x] = true;
	}
	//!Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu


	//Algorytm wzajemnej grafiki gruntu (postawiony na samym ko�cu - po wszystkich dzia�aniach na gruncie)
	for (int i = 0; i < this->ground.size(); i++)
		this->ground[i].graphicsCustomize(sf::Vector2u(this->width, this->height), is_ground, grid_size);
	//Pozosta�e algorytmy wzajemnej grafiki
	for (int i = 0; i < this->fluid.size(); i++)
		this->fluid[i].graphicsCustomize(this->world_type, sf::Vector2u(this->width, this->height), to_fluid, grid_size);


	//GENERATOR POZIOMU (POWER-UP'Y, T�O I NPC)
	//POWER-UPS
	//Ilo�c power-up'�w, kt�re maj� si� pojawi� w poziomie
	short pu_count = rand() % 3 + 2;
	//Je�eli ilo�� power-up'�w jest wi�ksza od ilo�ci miejsc spawn�w power-up'�w, to ilo�� power-up'�w jest r�wna ilo�ci miejsc spawnu power-up'�w (pojawi� si� na ka�dym spawnie)
	if (spawn_pu_pos.size() < pu_count)
		pu_count = spawn_pu_pos.size();
	//P�tla tworzenia power-up'�w
	for (int i = 0; i < pu_count; i++)
	{
		unsigned short spawn = rand() % spawn_pu_pos.size();
		this->power_up.push_back(spawn_pu_pos[spawn]);
		spawn_pu_pos.erase(spawn_pu_pos.begin() + spawn);
	}


	//NPC-Y
	//P�tla tworzenia NPC-�w
	int number_of_npcs = (float)(50 + 5.5f * (this->level_number - 1)) * ((float)(modulators[0] ? rand() % 16 + 5 : 10) / 10);
	for (int i = 0; i < number_of_npcs; i++)
	{
		//Losowanie ID NPC-a
		int random = randomNPCID(this->world_type, profile);

		//Tymczasowy NPC kt�ry b�dzie p�niej dopisany do wektora NPC-�w (gdy zotanie dopasowany do poziomu; aktualnie nie mo�e by� ju� dopisany i zmieniany, gdy� algorytm sprawdza�by, czy koliduje sam ze sob�)
		cNPC temp_npc(this->physics_world, this->world_type, modulators, random, this->randomPosition(416, this->width), (rand() % 2 ? DIR_LEFT : DIR_RIGHT));

		bool end = false;	//Nie przydzielono pozycji

		//Dla NPC-�w poruszaj�cych si� po sztywnych obiektach
		if (!temp_npc.getFeatures().flying && !temp_npc.getFeatures().swimming)
		{
			//P�tla trwa tak d�ugo a� NPC nie znajdzie si� dok�adanie nad powierzchni� sztywnego obiektu
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition(416, this->width));

				//Je�eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je�eli pod NPC-em znajduje si� sztywny obiekt (W przypadku NPC-�w chodz�cych)
					if (temp_npc.isCollisionOnGrid(is_solid, grid_size) || temp_npc.isCollisionOnGrid(is_npc, grid_size))
					{
						temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y - 32));
						end = true;
					}
				}
			}
		}
		//Dla lataj�cych NPC-�w
		else if (temp_npc.getFeatures().flying && !temp_npc.getFeatures().swimming)
		{
			//P�tla trwa tak d�ugo a� NPC nie znajdzie si� odpowiednio wysoko nad powierzchni� sztywnego obiektu
			while (!end)
			{

				sf::Vector2f main_pos = this->randomPosition(416, this->width);		//G��wna pozycja - pozycja pocz�tkowa NPC-a
				temp_npc.setAllPositions(main_pos);

				//Je�eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je�eli pod NPC-em r�wnie� nie znajduje si� sztywny obiekt
					if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
					{
						for (int i = 0; i < 2; i++)		//P�tla powtarza si� 2 razy - dok�adnie jeszcze tyle razy pod ni� mo�e nie by� gruntu
						{
							temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

							if (temp_npc.isCollisionOnGrid(is_solid, grid_size) || temp_npc.isCollisionOnGrid(is_npc, grid_size))	//Je�eli w tym miejscu znajduje si� sztywny obiekt, to mo�e nad nim lata�
							{
								temp_npc.setAllPositions(main_pos);
								end = true;

								break;
							}
						}
						//Je�eli mimo tylu powt�rze� p�tli nie znaleziono gruntu, to algorytm wyszukuje now� pozycj�
					}
				}
			}
		}
		//Dla p�ywaj�cych NPC-�w
		else
		{
			//P�tla trwa tak d�ugo a� NPC nie znajdzie si� w p�ynie
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition(416, this->width));

				//Je�eli NPC znajduje si� w wodzie
				if (temp_npc.isCollisionOnGrid(is_fluid, grid_size))
					end = true;
			}
		}

		sf::Vector2i pos_on_grid((temp_npc.getPosition().x - temp_npc.getOrigin().x) / 32, (temp_npc.getPosition().y - temp_npc.getOrigin().y) / 32);
		is_npc[pos_on_grid.y * grid_size.x + pos_on_grid.x] = true;

		this->npc.push_back(temp_npc);
	}

	//OBIEKTY W TLE
	//P�tla tworzenia obiekt�w w tle
	for (int i = 0; i < 100; i++)
	{
		eBackgroundType type;

		//Losowanie typu grafiki
		int random = rand() % 1001;	//0,0% - 100,0%

		switch (this->world_type)
		{
		case WORLD_UNDERWATER:
		{
			if (random < 550)
				type = BG_FLUID_DIVE;
			else if (random < 800)
				type = BG_FLUID_LYING;
			else
				type = BG_GROUND_INSIDE;
			break;
		}
		case WORLD_ICE_LAND:		//Bez wody
		{
			if (random < 250)	//25% szans na obiekt w powietrzu
				type = BG_FLYING;
			else if (random < 775)	//47.5% szans na obiekt na gruncie
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_GROUND_LYING;		break;}
				case 1: {type = BG_GROUND_LEVITATING;	break;}
				case 2: {type = BG_TRUNK;				break;}
				}
			}
			else	//27.5% szans na obiekt w gruncie
				type = BG_GROUND_INSIDE;

			break;
		}
		case WORLD_DESERT:		//Bez wody, ma�o przedmiot�w w gruncie
		{
			if (random < 250)	//25% szans na obiekt w powietrzu
				type = BG_FLYING;
			else if (random < 925)	//62.5% szans na obiekt na gruncie
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_GROUND_LYING;		break;}
				case 1: {type = BG_GROUND_LEVITATING;	break;}
				case 2: {type = BG_TRUNK;				break;}
				}
			}
			else	//15% szans na obiekt w gruncie
				type = BG_GROUND_INSIDE;

			break;
		}
		default:
		{
			if (random < 250)	//25% szans na obiekt w powietrzu
				type = BG_FLYING;
			else if (this->fluid.size() > 0 && random < 450)	//20% szans na obiekt w wodzie (o ile woda w og�le istnieje)
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_FLUID_LYING;	break;}
				case 1: {type = BG_FLUID_DIVE;	break;}
				case 2: {type = BG_FLUID_FLOAT;	break;}
				}
			}
			else if (random < 850 - (fluid.empty() ? 125 : 0))	//40% szans na obiekt na gruncie, chyba �e woda nie istnieje, to 47.5%
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_GROUND_LYING;		break;}
				case 1: {type = BG_GROUND_LEVITATING;	break;}
				case 2:
				{
					if (this->world_type == WORLD_OVERWORLD)
						type = BG_TRUNK;
					else if (this->world_type == WORLD_UNDERGROUND)	//W podziemiach nie generuj� si� drzewa (je�eli b�d� si� mia�y generowa�, to trzeba zmieni� ich spos�b generowania, bo w podziemiach jest mnie miejsca i drzewo mo�e w og�le nie znale�� miejsca generowania si�)
						type = BG_CEILING;
					else
						type = BG_GROUND_LEVITATING;
					break;
				}
				}
			}
			else	//15% szans na obiekt w gruncie, chyba �e woda nie istnieje, to 27.5%
				type = BG_GROUND_INSIDE;

			break;
		}
		}
		//!Losowanie typu grafiki

		//Tymczasowy obiekt w tle kt�ry b�dzie p�niej dopisany do wektora obiekt�w w tle (gdy zotanie dopasowany do poziomu; aktualnie nie mo�e by� ju� dopisany i zmieniany, gdy� algorytm sprawdza�by, czy koliduje sam ze sob�)
		cBackgroundObject temp_bg_obj(this->world_type, type, this->randomPosition(0, this->width));

		//Sprawdzanie, czy obiekt w tle nie znajduje si� w gruncie
		switch (type)
		{
		case BG_GROUND_LYING:
		case BG_GROUND_LEVITATING:
		case BG_TRUNK:
		case BG_TOP_TREE:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle nie jest zakopany w gruncie i nie znajduje si� w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_ground, grid_size) && !temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle znajduje si� grunt (b�dzie le�a� na gruncie)
					if (temp_bg_obj.isCollisionOnGrid(is_ground, grid_size))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case BG_FLYING:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle nie jest zakopany w gruncie i nie znajduje si� w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_ground, grid_size) && !temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle nie znajduje si� grunt (b�dzie lewitowa� w powietrzu)
					if (!temp_bg_obj.isCollisionOnGrid(is_ground, grid_size))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case BG_GROUND_INSIDE:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle jest zakopany w gruncie
				if (temp_bg_obj.isCollisionOnGrid(is_ground, grid_size))
					end = true;
			}
			break;
		}
		case BG_FLUID_LYING:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle nie jest zakopany w sztywnym obiekcie i znajduje si� w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_solid, grid_size) && temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle znajduje si� grunt (b�dzie le�a� na gruncie)
					if (temp_bg_obj.isCollisionOnGrid(is_ground, grid_size))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case BG_FLUID_DIVE:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle znajduje si� w wodzie i nie znajduje si� w sztywnym obiekcie)
				if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
					{
						temp_bg_obj.move(0, -32);

						//Je�eli wy�ej jest woda (nie p�ywa, a znajduje si� "wewn�trz" wody
						if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
						{
							temp_bg_obj.move(0, 32);
							end = true;
						}
					}
				}
			}
			break;
		}
		case BG_FLUID_FLOAT:
		{
			bool end = false;
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle znajduje si� w wodzie
				if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je�eli wy�ej nie ma wody
					if (!temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
					{
						temp_bg_obj.move(0, 26);	//Obni�ono o nieco mniej, �eby obiekt "p�ywa�" na wodzie
						end = true;
					}
				}
			}
			break;
		}
		case BG_CEILING:
		{
			bool end = false;
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition(0, this->width));

				//Je�eli obiekt w tle nie znajduje si� w sztywnym obiekcie
				if (!temp_bg_obj.isCollisionOnGrid(is_solid, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je�eli wy�ej jest sztywny obiekt
					if (temp_bg_obj.isCollisionOnGrid(is_solid, grid_size))
					{
						temp_bg_obj.move(0, 32);
						end = true;
					}
				}
			}
			break;
		}
		}
		this->background_obj.push_back(temp_bg_obj);
	}

	//Generowanie graczy
	if (player.size() == 0)
	{
		for (int i = 0; i < number_of_players; i++)
		{
			cKnight *knight = new cKnight(this->physics_world, this->world_type, this->randomPosition(0, 192), i + 1, modulators);
			cArcher *archer = new cArcher(this->physics_world, this->world_type, this->randomPosition(0, 192), i + 1, modulators);
			cSpy *spy = new cSpy(this->physics_world, this->world_type, this->randomPosition(0, 192), i + 1, modulators);
			cSorceress *sorceress = new cSorceress(this->physics_world, this->world_type, this->randomPosition(0, 192), i + 1, modulators);

			cCharacter *temp_player = NULL;
			switch (character[i])
			{
			case 1: {temp_player = archer; break;}
			case 2: {temp_player = spy; break;}
			case 3: {temp_player = sorceress; break;}
			default: {temp_player = knight; break;}
			}

			bool end = false;	//Nie przydzielono pozycji

			//P�tla trwa tak d�ugo a� posta� nie znajdzie si� dok�adanie nad powierzchni� sztywnego obiektu
			while (!end)
			{
				temp_player->setAllPositions(this->randomPosition(0, 192));

				//Je�eli posta� nie jest zakopany w sztywnym obiekcie
				if (!temp_player->isCollisionOnGrid(is_solid, grid_size) && !temp_player->isCollisionOnGrid(is_npc, grid_size))
				{
					temp_player->setAllPositions(sf::Vector2f(temp_player->getPosition().x, temp_player->getPosition().y + 32));

					//Je�eli pod postaci� znajduje si� sztywny obiekt
					if (temp_player->isCollisionOnGrid(is_solid, grid_size) || temp_player->isCollisionOnGrid(is_npc, grid_size))
					{
						temp_player->setAllPositions(sf::Vector2f(temp_player->getPosition().x, temp_player->getPosition().y - 32));
						end = true;
					}
				}
			}

			this->player.push_back(temp_player);
		}

		for (unsigned short i = 0; i < number_of_players; i++)
			player[i]->initPet();
	}
	else
	{
		for (unsigned short i = 0; i < this->player.size(); i++)
		{
			this->player[i]->bodyRecreate(this->physics_world, this->world_type);	//Cia�a zawsze musz� si� odnowia� (pow�d: m.in. oddzia�ywanie na l�d)

			bool end = false;	//Nie przydzielono pozycji

			//P�tla trwa tak d�ugo a� posta� nie znajdzie si� dok�adanie nad powierzchni� sztywnego obiektu
			while (!end)
			{
				this->player[i]->setAllPositions(this->randomPosition(0, 192));

				//Je�eli posta� nie jest zakopana w sztywnym obiekcie
				if (!this->player[i]->isCollisionOnGrid(is_solid, grid_size) && !this->player[i]->isCollisionOnGrid(is_npc, grid_size))
				{
					this->player[i]->setAllPositions(sf::Vector2f(this->player[i]->getPosition().x, this->player[i]->getPosition().y + 32));

					//Je�eli pod postaci� znajduje si� sztywny obiekt
					if (this->player[i]->isCollisionOnGrid(is_solid, grid_size))
					{
						this->player[i]->setAllPositions(sf::Vector2f(this->player[i]->getPosition().x, this->player[i]->getPosition().y - 32));
						end = true;
					}
				}
			}

			this->player[i]->rebirth();
		}
	}
	//!Generowanie graczy

	//Tworzenie zmiennych startowych
	this->initial_bbs_size = this->bonus_block.size();

	//Usuwanie zb�dnych tablic dynamicznych
	delete[] is_solid;
	delete[] is_ground;
	delete[] is_fluid;
	delete[] to_fluid;
	delete[] is_npc;

	this->saveGame(win, profile, slot_name, modulators);
}

bool cMap::movements(sf::RenderWindow &win, sf::View &view, bool *modulators, cScoreboard &scoreboard, cProfile &profile, std::string &slot_name)
{
	//MENU PAUZY
	static bool key_pressed = true;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		key_pressed = false;
	else if (!key_pressed)
	{
		key_pressed = true;
		if (!menuPause(win))
			return false;
	}

	//LICZNIKI/TIMERY
	if (this->experience_countdown > 0)
		this->experience_countdown--;

	//TWORZENIE Z�OTEGO BONUSOWEGO BLOKU
	if (this->bonus_block.size() == 0 && !this->golden_bb_created)
	{
		this->golden_bb_created = true;
		this->bonus_block.push_back(cBonusBlock(this->physics_world, t_gold_bonus_block[0], sf::Vector2f(this->width - 160 - 16, this->height - 128 - 16), 40, 55));
	}

	//SKARBY
	for (int i = this->treasure.size() - 1; i >= 0; i--)
	{
		this->treasure[i].step(this->world_type, sf::Vector2i(this->width, this->height), this->fluid_tab);

		//Wypad� poza obszar mapy
		if (this->treasure[i].getPosition().y - this->treasure[i].getOrigin().y > this->height)
			this->treasure.erase(this->treasure.begin() + i);
	}

	//POCISKI
	for (int i = this->bullet.size() - 1; i >= 0; i--)
	{
		this->bullet[i].step(this->world_type, sf::Vector2i(this->width, this->height), fluid_tab);
		this->bullet[i].specialCollisions(this->physics_world, this->world_type, modulators, this->player, this->npc, this->treasure, this->bonus_block);

		if (this->bullet[i].isDestroyed())
		{
			this->physics_world.DestroyBody(bullet[i].getBody());
			this->bullet.erase(bullet.begin() + i);
		}
	}

	//NPC-Y
	sf::FloatRect view_rect;	//view_rect to prostok�t, kt�ry zape�nia aktualny widok kamery. Dzi�ki temu mo�na sprawdzi�, czy NPC-y znajduj� si� w polu widoku i maj� zacz�� si� porusza�.
	view_rect.left = view.getCenter().x - view.getSize().x / 2;
	view_rect.top = view.getCenter().y - view.getSize().y / 2;
	view_rect.width = view.getCenter().x + view.getSize().x / 2;
	view_rect.height = view.getCenter().y + view.getSize().y / 2;

	for (int i = this->npc.size() - 1; i >= 0; i--)
	{
		this->npc[i].step(this->world_type, sf::Vector2i(this->width, this->height), fluid_tab, view_rect);

		//Wyszed� poza obszar mapy
		if (this->npc[i].getPosition().y - this->npc[i].getOrigin().y > this->height)
		{
			this->npc[i].kill();
			this->npc.erase(this->npc.begin() + i);
		}
	}

	//POSTACIE GRACZY
	bool are_all_players_dead = true;
	for (unsigned int i = 0; i < this->player.size(); i++)
	{
		if (!this->player[i]->isDead())
		{
			are_all_players_dead = false;

			this->player[i]->control(this->physics_world, this->world_type, this->bullet);
			this->player[i]->specialCollisions(this->physics_world, this->world_type, modulators, this->npc, this->power_up, this->treasure, this->fluid, this->trampoline, this->ladder, this->bonus_block);
			this->player[i]->applyPhysics(this->world_type, this->fluid_tab, sf::Vector2i(this->width / 32, this->height / 32));
			this->player[i]->checkIndicators(this->physics_world, this->world_type, this->player, this->bullet);
			this->player[i]->move(win, sf::Vector2f(this->width, this->height));

			//Rozpocz�cie nast�pnego poziomu
			if (this->player[i]->getPosition().x - this->player[i]->getOrigin().x > this->width)
			{
				for (unsigned short j = 0; j < this->player.size(); j++)
					if (!this->player[j]->isDead())
						this->player[j]->addStatsForEndOfLevel(this->level_number, this->experience_countdown);

				cShop shop(this->player, modulators);
				if (!shop.shopMenu(win) || !menuSkillTree(win, this->player))
				{
					win.close();
					return true;
				}

				this->levelGenerator(win, profile, slot_name, player.size(), modulators, false, true);

				return true;
			}
		}
	}

	//Rozpocz�cie poziomu od nowa, je�eli wszyscy gracze s� martwi
	if (are_all_players_dead)
	{
		//Zako�czenie gry, je�eli �aden z graczy nie ma wi�cej �y�
		bool no_more_life = true;
		for (short i = 0; i < this->player.size(); i++)
		{
			if (this->player[i]->hasLife())
			{
				no_more_life = false;
				break;
			}
		}

		if (no_more_life)
		{
			//Usuwanie slotu zapisu
			profile.deleteSaveSlot(win, slot_name);

			//Zapisywanie si� do tabeli wynik�w
			for (int i = 0; i < this->player.size(); i++)
			{
				win.clear();
				if (scoreboard.isSufficientlyHighScore(this->player[i]->getScore()))
				{
					sf::String description_str = L"Player ";
					description_str += uIntToStr(i + 1);
					description_str += " reached a new high score! Enter your name:";

					std::string name = textDialog(win, "New high score!", description_str);

					scoreboard.appendScore(name, this->player[i]->getScore());
				}
			}

			std::string path = "hiscores" + uIntToStr(this->player.size()) + ".dat";
			scoreboard.saveScoreboard(path);

			//Nagroda za gr�
			unsigned int cash = 0;

			unsigned int allScore = 0;
			for (int i = 0; i < this->player.size(); i++)
				allScore += this->player[i]->getScore();
			cash = allScore / 1000;

			sf::String description_str = "You get ";
			description_str += uIntToStr(cash);
			description_str += " cash for the game.";

			okDialog(win, "Award for the game", description_str);

			profile.addCash(cash);
			profile.saveProfile(win);

			//Koniec gry
			return false;
		}

		this->levelGenerator(win, profile, slot_name, player.size(), modulators, true, false);
		return true;
	}

	for (int i = npc.size() - 1; i >= 0; i--)
		if (npc[i].isDead())
			this->npc.erase(this->npc.begin() + i);

	this->physics_world.Step((float)1 / 60, 8, 3);
	return true;
}

bool cMap::saveGame(sf::RenderWindow &win, cProfile &profile, std::string slot_name, bool *modulators)
{
	//Tworzenie katalogu na save'y
	std::string path = "profiles/" + profile.getName() + "/saves";

	#ifdef _WIN32
        CreateDirectory(path.c_str(), 0);
    #endif
    #ifdef linux
        std::string linux_path="mkdir -p " + path;
        system(linux_path.c_str());
    #endif

	//Tworzenie pliku zapisu
	std::fstream save_file;
	save_file.open("profiles/" + profile.getName() + "/saves/" + slot_name + ".dat", std::ios::out | std::ios::trunc | std::ios::binary);

	if (save_file.is_open())
	{
		save_file.write((char*)&this->level_number, sizeof(unsigned int));
		save_file.write((char*)&this->world_type, sizeof(eWorld));

		//Modulatory
		save_file.write((char*)&g_all_modulators, sizeof(int));
		for (int i = 0; i < g_all_modulators; i++)
			save_file.write((char*)&modulators[i], sizeof(bool));

		//Postacie graczy
		save_file.write((char*)&this->player_number, sizeof(int));

		for (int i = 0; i < this->player_number; i++)
		{
			eCharacter character_type = this->player[i]->getCharacterType();
			unsigned short lvl = this->player[i]->getLevel();
			unsigned int exp = this->player[i]->getExperience();
			unsigned short skill_points = this->player[i]->getSkillPoints();

			int skills_number = 4;
			unsigned short number_of_skill[4];
			for (int j = 0; j < 4; j++)
				number_of_skill[j] = this->player[i]->getNumberOfSkill(j);

			int bonus_number = 2;
			unsigned int bonus[2];
			for (int j = 0; j < 2; j++)
				bonus[j] = this->player[i]->getBonus(j);

			unsigned short life = this->player[i]->getLife();
			unsigned short hp = this->player[i]->getHP();
			unsigned int score = this->player[i]->getScore();
			unsigned int cash = this->player[i]->getCash();
			bool has_taser = this->player[i]->hasTaser();


			save_file.write((char*)&character_type, sizeof(eCharacter));
			save_file.write((char*)&lvl, sizeof(unsigned short));
			save_file.write((char*)&exp, sizeof(unsigned int));
			save_file.write((char*)&skill_points, sizeof(unsigned short));

			save_file.write((char*)&skills_number, sizeof(int));
			for (int j = 0; j < skills_number; j++)
				save_file.write((char*)&number_of_skill[j], sizeof(unsigned short));

			save_file.write((char*)&bonus_number, sizeof(int));
			for (int j = 0; j < bonus_number; j++)
				save_file.write((char*)&bonus[j], sizeof(unsigned int));

			save_file.write((char*)&life, sizeof(unsigned short));
			save_file.write((char*)&hp, sizeof(unsigned short));
			save_file.write((char*)&score, sizeof(unsigned int));
			save_file.write((char*)&cash, sizeof(unsigned int));
			save_file.write((char*)&has_taser, sizeof(bool));
		}
	}
	else
	{
		okDialog(win, "Error 10", "Can't save the game!");
		return false;
	}
	save_file.close();
	return true;
}

bool cMap::loadGame(sf::RenderWindow &win, cProfile &profile, std::string slot_name, bool *modulators)
{
	this->player.clear();
	this->reserve_sector.clear();

	std::fstream load_file;
	load_file.open("profiles/" + profile.getName() + "/saves/" + slot_name + ".dat", std::ios::in | std::ios::binary);

	if (load_file.is_open())
	{
		load_file.read((char*)&this->level_number, sizeof(unsigned int));
		load_file.read((char*)&this->world_type, sizeof(eWorld));

		//Modulatory
		int modulators_size;
		load_file.read((char*)&modulators_size, sizeof(int));
		for (int i = 0; i < modulators_size; i++)
			load_file.read((char*)&modulators[i], sizeof(bool));

		//Postacie graczy
		load_file.read((char*)&this->player_number, sizeof(int));

		for (int i = 0; i < this->player_number; i++)
		{
			eCharacter character_type;
			unsigned short lvl, skill_points, number_of_skill[4], life, pet_hp;
			unsigned int exp, bonus[2], score, cash;
			int skills_number, bonus_number;
			bool has_taser;

			load_file.read((char*)&character_type, sizeof(eCharacter));
			load_file.read((char*)&lvl, sizeof(unsigned short));
			load_file.read((char*)&exp, sizeof(unsigned int));
			load_file.read((char*)&skill_points, sizeof(unsigned short));

			load_file.read((char*)&skills_number, sizeof(int));
			for (int i = 0; i < skills_number; i++)
				load_file.read((char*)&number_of_skill[i], sizeof(unsigned short));
			for (int i = skills_number; i < 2; i++)
				number_of_skill[i] = 0;

			load_file.read((char*)&bonus_number, sizeof(int));
			for (int i = 0; i < bonus_number; i++)
				load_file.read((char*)&bonus[i], sizeof(unsigned int));
			for (int i = bonus_number; i < 2; i++)
				bonus[i] = 0;

			load_file.read((char*)&life, sizeof(unsigned short));
			load_file.read((char*)&pet_hp, sizeof(unsigned short));
			load_file.read((char*)&score, sizeof(unsigned int));
			load_file.read((char*)&cash, sizeof(unsigned int));
			load_file.read((char*)&has_taser, sizeof(bool));

			cKnight *knight = new cKnight(this->physics_world, this->world_type, sf::Vector2f(0.0f, 0.0f), i + 1, modulators);
			cArcher *archer = new cArcher(this->physics_world, this->world_type, sf::Vector2f(0.0f, 0.0f), i + 1, modulators);
			cSpy *spy = new cSpy(this->physics_world, this->world_type, sf::Vector2f(0.0f, 0.0f), i + 1, modulators);
			cSorceress *sorceress = new cSorceress(this->physics_world, this->world_type, sf::Vector2f(0.0f, 0.0f), i + 1, modulators);

			cCharacter *temp_player = NULL;
			switch (character_type)
			{
			case CHARACTER_KNIGHT: {temp_player = knight; break;}
			case CHARACTER_ARCHER: {temp_player = archer; break;}
			case CHARACTER_SPY: {temp_player = spy; break;}
			case CHARACTER_SORCERESS: {temp_player = sorceress; break;}
			}

			this->player.push_back(temp_player);
			this->player[i]->initPet();
			this->player[i]->loadCharacter(lvl, exp, skill_points, number_of_skill, pet_hp, bonus, life, score, cash, has_taser);
		}
	}
	else
	{
		okDialog(win, "Error 11", "Can't load the game!");
		return false;
	}
	load_file.close();
	return true;
}

void cMap::draw(sf::RenderWindow &win, sf::View &view)
{
	//Przesuwanie kamery na posta� gracza
	sf::Vector2f view_center_point(0.0f, 0.0f);
	short how_many_player_alive = 0;
	for (int i = 0; i < this->player_number; i++)
	{
		if (!this->player[i]->isDead())
		{
			view_center_point.x += this->player[i]->getPosition().x;
			view_center_point.y += this->player[i]->getPosition().y;
			how_many_player_alive++;
		}
	}
	view_center_point.x /= how_many_player_alive;
	view_center_point.y /= how_many_player_alive;

	view.setCenter(view_center_point);
	if (view.getCenter().x > this->getWidth() - 400)
		view.setCenter(this->getWidth() - 400, view.getCenter().y);
	else if (view.getCenter().x < 400)
		view.setCenter(sf::Vector2f(400, view.getCenter().y));
	if (view.getCenter().y < 300)
		view.setCenter(sf::Vector2f(view.getCenter().x, 300));
	else if (view.getCenter().y > this->getHeight() - 300)
		view.setCenter(sf::Vector2f(view.getCenter().x, this->getHeight() - 300));
	win.setView(view);

	//Przesuwanie ekranu
	this->background[0].setPosition((view.getCenter().x - 400) / 1.2 + (((int)((view.getCenter().x - 400) / 1.2 / 5) / this->background[0].getTextureRect().width) * this->background[0].getTextureRect().width), (view.getCenter().y - 300) / 1.2);
	background[1].setPosition(this->background[0].getPosition().x + this->background[0].getTextureRect().width, this->background[0].getPosition().y);

	//Rysowanie obiekt�w mapy na oknie programu
	for (unsigned short i = 0; i < 2; i++)
		win.draw(this->background[i]);
	for (unsigned int i = 0; i < this->fluid.size(); i++)
		win.draw(this->fluid[i]);
	for (unsigned int i = 0; i < this->ground.size(); i++)
		win.draw(this->ground[i]);
	for (unsigned int i = 0; i < this->background_obj.size(); i++)
		if (!this->background_obj[i].front)
			this->background_obj[i].drawAllGraphics(win);
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
		win.draw(this->bonus_block[i]);
	for (unsigned int i = 0; i < this->block.size(); i++)
		win.draw(this->block[i]);
	for (unsigned int i = 0; i < this->ladder.size(); i++)
		win.draw(this->ladder[i]);
	for (unsigned int i = 0; i < this->trampoline.size(); i++)
		win.draw(this->trampoline[i]);
	for (unsigned int i = 0; i < this->treasure.size(); i++)
		win.draw(this->treasure[i]);
	for (unsigned int i = 0; i < this->power_up.size(); i++)
		win.draw(this->power_up[i]);
	for (unsigned short i = 0; i < this->bullet.size(); i++)
		win.draw(bullet[i]);
	for (unsigned int i = 0; i < this->npc.size(); i++)
		win.draw(this->npc[i]);
	for (unsigned short i = 0; i < this->player.size(); i++)
		if (!this->player[i]->isDead())
		{
			this->player[i]->draw(win);
			if (this->player[i]->isPetAlive())
				win.draw(this->player[i]->getPet());
		}
	for (unsigned int i = 0; i < this->background_obj.size(); i++)
		if (this->background_obj[i].front)
			this->background_obj[i].drawAllGraphics(win);
	//Statystyki graczy
	this->player[0]->drawStats(win, sf::Vector2f(16, 16));
	if (this->player_number >= 2)
	{
		this->player[1]->drawStats(win, sf::Vector2f(148, 16));
		if (this->player_number >= 3)
		{
			this->player[2]->drawStats(win, sf::Vector2f(536, 16));
			if (this->player_number == 4)
				this->player[3]->drawStats(win, sf::Vector2f(668, 16));
		}
	}

	//Numer poziomu
	sf::String str(L"Level: ");

	std::string no;
	std::stringstream ss;
	ss << this->level_number;
	no = ss.str();
	ss.str("");
	str += no;

	sf::Text text(str, font[0], 48);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(view.getCenter().x, 32);

	win.draw(text);

	//Odliczanie do�wiadczenia
	str = L"Extra experience: ";

	ss << (this->experience_countdown + g_framerate_limit - 1) / g_framerate_limit;
	no = ss.str();
	ss.str("");
	str += no;

	text.setString(str);
	text.setCharacterSize(20);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(view.getCenter().x, 80);

	win.draw(text);

	//Ilo�� zniszczonych / ca�kowita ilo�� bonusowych blok�w
	str = L" ";

	if (!this->golden_bb_created)
		ss << this->initial_bbs_size - this->bonus_block.size();
	else
		ss << this->initial_bbs_size;
	no = ss.str();
	ss.str("");
	str += no;

	str += "/";

	ss << this->initial_bbs_size;
	no = ss.str();
	ss.str("");
	str += no;

	text.setString(str);
	text.setCharacterSize(20);
	text.setFillColor(this->golden_bb_created ? sf::Color(255, 215, 0) : sf::Color(255, 255, 255));
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(view.getCenter().x, 100);

	//Grafika bonusowego bloku
	sf::Sprite s_bonus_block(t_bonus_block[0]);
	s_bonus_block.setScale(0.5f, 0.5f);
	s_bonus_block.setOrigin(s_bonus_block.getGlobalBounds().width, s_bonus_block.getGlobalBounds().height / 2);
	s_bonus_block.setColor(sf::Color(255, 255, 255, 224));
	s_bonus_block.setPosition(view.getCenter().x - 32, 100);

	s_bonus_block.setPosition(view.getCenter().x - (s_bonus_block.getGlobalBounds().width + text.getGlobalBounds().width) / 2, 100);

	win.draw(text);
	win.draw(s_bonus_block);
}

unsigned int cMap::getWidth()
{
	return this->width;
}

unsigned int cMap::getHeight()
{
	return this->height;
}

sf::Vector2f cMap::randomPosition(unsigned int min_x, unsigned int max_x)
{
	return sf::Vector2f(min_x + (rand() % ((max_x - min_x) / 32) * 32 + 16), this->height - (rand() % (this->height / 32) * 32 + 16));
}

void cMap::destroy(bool destroy_players)
{
	if (this->ground.size() > 0)
		this->physics_world.DestroyBody(this->ground[0].getBody());		//Ca�a ziemia posiada jedno cia�o
	if (this->fluid.size() > 0)
		this->physics_world.DestroyBody(this->fluid[0].getBody());		//Ca�y p�yn posiada jedno cia�o

	for (unsigned int i = 0; i < this->block.size(); i++)
		this->physics_world.DestroyBody(this->block[i].getBody());
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
		this->physics_world.DestroyBody(this->bonus_block[i].getBody());
	for (unsigned int i = 0; i < this->npc.size(); i++)
		this->physics_world.DestroyBody(this->npc[i].getBody());
	for (unsigned int i = 0; i < this->treasure.size(); i++)
		if (this->treasure[i].isPhysical())
			this->physics_world.DestroyBody(this->treasure[i].getBody());
	for (unsigned int i = 0; i < this->trampoline.size(); i++)
		this->physics_world.DestroyBody(this->trampoline[i].getBody());
	for (unsigned int i = 0; i < this->bullet.size(); i++)
		this->physics_world.DestroyBody(this->bullet[i].getBody());

	if (destroy_players)
		for (unsigned short i = 0; i < this->player.size(); i++)
			if (!player[i]->isDead())
				this->physics_world.DestroyBody(this->player[i]->getBody());

	for (unsigned int i = 0; i < this->physics_world.GetJointCount(); i++)
		this->physics_world.DestroyJoint(this->physics_world.GetJointList());

	delete[] this->fluid_tab;

	this->ground.clear();
	this->fluid.clear();
	this->block.clear();
	this->bonus_block.clear();
	this->npc.clear();
	this->treasure.clear();
	this->background_obj.clear();
	this->trampoline.clear();
	this->power_up.clear();
	this->ladder.clear();
	this->bullet.clear();
	if (destroy_players)
		this->player.clear();
}
