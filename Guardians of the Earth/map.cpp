#include "map.h"
#include <iostream>

cMap::cMap(eWorld world, short number_of_players) :physics_world(b2Vec2(0.0f, 10.0f))
{
	this->level_number = 1;
	this->world_type = world;
	this->prev_sector.clear();
	levelGenerator(number_of_players, false, false);
}

void cMap::levelGenerator(short number_of_players, bool refresh, bool next_level)
{
	system("CLS");
	this->player_number = number_of_players;
	
	if (refresh || next_level)
		this->destroy(false);
	if (next_level)
	{
		this->level_number++;
		this->world_type = (eWorld)(rand() % 5);
	}
	if (!refresh)
		this->reserve_sector.clear();

	groundsInit(&(this->physics_world));
	fluidInit(&(this->physics_world));

	this->width = 0;
	this->height = 600;
	
	this->x_generate = 0;

	//GENERATOR POZIOMU (TEREN)
	//Przypisanie tekstury do t³a (s¹ 2 - jedno t³o le¿y za drugim (dziêki temu t³o mo¿e siê przesuwaæ))
	for (int i = 0; i < 2; i++)
	{
		background[i].setTextureRect(sf::IntRect(0, 0, t_background[this->world_type].getSize().x, t_background[this->world_type].getSize().y));
		background[i].setTexture(t_background[this->world_type]);
	}

	//Stworzenie tablicy opisuj¹cej, jak wiele w poziomie jest sektorów o danym ID (w celach debugowania)
	std::cout << "Ilosc sektorow w tym typie swiata: " << how_many_sectors[this->world_type] << "\n\n\n";
	int *how_many = new int[how_many_sectors[this->world_type]];
	for (int i = 0; i < how_many_sectors[this->world_type]; i++)
		how_many[i] = 0;

	//Wektor zmiennych odpowiedzialnych za pozycje spawnów bonusów
	std::vector <sf::Vector2f> spawn_pu_pos;

	//Utworzenie zmiennej sektor
	cSector sector;

	//Pêtla tworzenia terenu
	clock_t time_map = clock();
	for (int i = 0; i < 50; i++)	//Iloœæ sektorów znajduj¹cych siê na mapie (GOTO zamieniæ na ogóln¹ d³ugoœæ mapy (¿eby poziomy by³y podobnej d³ugoœci))
	{
		if (!refresh)	//Tworzenie poziomu od podstaw
		{
			std::cout << i << " ";
			std::string sector_id;
			//Pêtla wyszukiwania pasuj¹cego sektora
			do
			{
				sector.loadRandomSector(this->world_type, sector_id);
				if (i == 0)	//Pierwszy sektor zawsze pasuje
					break;
			} while (!sector.isSectorFitted(this->world_type, this->prev_sector, this->getHeight()));

			reserve_sector.push_back(sector);
			int s_id = atoi((char*)sector_id.c_str());
			std::cout << s_id << "\n";
			how_many[s_id - 1]++;

			//Przypisanie sektora poprzedniemu sektorowi - dziêki temu mo¿na bêdzie znowu wyszukiwaæ pasuj¹cy sektor
			this->prev_sector = sector;
		}
		else	//Skorzystanie z gotowego poziomu (sektory)
			sector = this->reserve_sector[i];

		//Dla œwiata podziemnego generator tworzy ponad sektorem warstwy gruntu
		if (this->world_type == WORLD_UNDERGROUND)
		{
			for (unsigned int i = 0; i < ceil((float)this->height / 32) - sector.getHeight(); i++)
				for (unsigned int j = 0; j < sector.getWidth(); j++)
				{
					this->ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 - (32 - this->height % 32)), this->world_type));
				}
		}
		//!Dla œwiata podziemnego generator tworzy ponad sektorem warstwy gruntu
		/*Poziomy podwodne nie s¹ wype³niane wod¹ gdy¿ by³oby to zbyt zasobo¿erne i w dodatku nie mo¿na by by³o dodaæ t³a.*/

		//Je¿eli sektor jest wy¿szy od aktualnej wysokoœci poziomu, to ca³y poziom staje siê wy¿szy (kamera mo¿e pod¹¿aæ wy¿ej)
		if (sector.getHeight() * 32 + 64 > this->height)
			this->height = sector.getHeight() * 32 + 64;

		int to_down = g_height - sector.getHeight() * 32;	//Przyrownanie calego terenu do podloza mapy (wysokosc mapy - wysokosc sektora)
		for (int i = 0; i < sector.getHeight(); i++)
			for (int j = 0; j < sector.getWidth(); j++)
			{
				switch (sector.getObject(j, i))
				{
				case eObjType::OBJECT_GROUND: {ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), this->world_type)); break;}
				case eObjType::OBJECT_BLOCK: {block.push_back(cBlock(&(this->physics_world), t_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				case eObjType::OBJECT_BONUS_BLOCK: {bonus_block.push_back(cBonusBlock(&(this->physics_world), t_bonus_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				case eObjType::OBJECT_FLUID:
				{
					if (this->world_type != WORLD_UNDERWATER)	//Œwiat podwodny jest ju¿ wype³niony wod¹, a obiekty wody tylko by niepotrzebnie spowalnia³y program (przy okazji teraz mo¿na dodaæ t³o)
						fluid.push_back(cFluid(this->world_type, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));
					break;
				}
				case eObjType::OBJECT_TREASURE: {treasure.push_back(cTreasure(&(this->physics_world), this->world_type, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_TRAMPOLINE: {trampoline.push_back(cTrampoline(&(this->physics_world), 1, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), 5.0f));	break;}
				case eObjType::OBJECT_POWER_UP: {spawn_pu_pos.push_back(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down));	break;}
				case eObjType::OBJECT_LADDER: {ladder.push_back(cLadder(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down))); break;}
				}
			}

		//Zwiêkszanie szerokoœci poziomu
		x_generate += sector.getWidth() * 32;
		this->width = x_generate;
	}
	time_map = clock() - time_map;


	//Wyœwietlanie na ekranie jak wiele jest sektorów o danym ID (W celach debugowania)
	std::cout << "\n\n\n";
	for (unsigned int i = 0; i < how_many_sectors[this->world_type]; i++)
		std::cout << i + 1 << " = " << how_many[i] << "\n";


	//Dostosowywanie obiektów mapy do ustawieñ mapy
	clock_t time_adjust = clock();
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
	for (unsigned int i = 0; i < spawn_pu_pos.size(); i++)		//Ta sama zasada dzia³ania, co w przypadku poprzednich pêtli
		spawn_pu_pos[i] = sf::Vector2f(spawn_pu_pos[i].x, spawn_pu_pos[i].y + this->height - g_height);
	time_adjust = clock() - time_adjust;
	//!Dostosowywanie obiektów mapy do ustawieñ mapy


	//Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu
	clock_t time_optimization = clock();
	sf::Vector2i grid_size(this->width / 32, this->height / 32);	//Wymiary siatki (wymiary poziomu podzielone przez 32)
	bool *is_solid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj¹ca za to, czy w danym punkcie znajduje siê sztywny obiekt (grunt, blok, ...) - dziêki temu mo¿na zoptymalizowaæ generowanie obiektów w poziomie
	bool *is_ground = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj¹ca za to, czy w danym punkcie znajduje siê grunt - dziêki temu mo¿na zoptymalizowaæ generowanie obiektów w poziomie
	bool *is_fluid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj¹ca za to, czy w danym punkcie znajduje siê p³yn - dziêki temu mo¿na zoptymalizowaæ generowanie obiektów w poziomie
	bool *to_fluid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj¹ca za to, czy w danym punkcie znajduje siê obiekt który ma wp³yw na wygl¹d p³ynu - dziêki temu mo¿na zoptymalizowaæ generowanie obiektów w poziomie
	bool *is_npc = new bool[grid_size.x * grid_size.y];		//Tablica odpowiadaj¹ca za to, czy w danym punkcie znajduje siê npc - dziêki temu mo¿na zoptymalizowaæ generowanie obiektów w poziomie

	//Wymagana oddzielna tablica dynamiczna dla wody w rozgryce, gdy¿ tablica ta jest wiêksza od pozosta³ych (o 1 w ka¿d¹ stronê)
	this->fluid_tab = new bool[(grid_size.x + 1) * (grid_size.y + 1)];	//Tablica odpowiedzialna za optymalizacjê rozgrywki pod wzglêdem p³ynów; +1 - mapa nie zawsze jest d³ugoœci podzielnej przez 32

	for (unsigned int i = 0; i < grid_size.y; i++)
		for (unsigned int j = 0; j < grid_size.x; j++)
		{
			is_solid[i * grid_size.x + j] = false;
			is_ground[i * grid_size.x + j] = false;
			to_fluid[i * grid_size.x + j] = false;
			is_npc[i * grid_size.x + j] = false;

			if (this->world_type == WORLD_UNDERWATER)		//W typie podwodnym wszêdzie jest woda
				is_fluid[i * grid_size.x + j] = true;
			else
				is_fluid[i * grid_size.x + j] = false;
		}
	//Dla p³ynów jest o 1 pole w ka¿d¹ stronê wiêcej
	for (unsigned int i = 0; i <= grid_size.y; i++)
		for (unsigned int j = 0; j <= grid_size.x; j++)
		{
			if (this->world_type == WORLD_UNDERWATER)		//W typie podwodnym wszêdzie jest woda
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
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W œwiecie underworld wszystko jest wod¹ (trzeba wy³¹czyæ pewne elementy, ¿eby nie pojawia³y siê na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->block.size(); i++)
	{
		sf::Vector2i pos = this->block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W œwiecie underworld wszystko jest wod¹ (trzeba wy³¹czyæ pewne elementy, ¿eby nie pojawia³y siê na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
	{
		sf::Vector2i pos = this->bonus_block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = false;	//W œwiecie underworld wszystko jest wod¹ (trzeba wy³¹czyæ pewne elementy, ¿eby nie pojawia³y siê na nich obiekty w tle)
	}
	for (unsigned int i = 0; i < this->fluid.size(); i++)
	{
		sf::Vector2i pos = this->fluid[i].posOnGrid(sf::Vector2i(32, 32));
		this->fluid_tab[pos.y * grid_size.x + pos.x] = true;
		to_fluid[pos.y * grid_size.x + pos.x] = true;
		is_fluid[pos.y * grid_size.x + pos.x] = true;

		//Dodatkowo je¿eli ostatnim elementem na osi X jest p³yn, to nastêpnym te¿ jest p³yn (naprawa b³êdów dotycz¹cych ostatnich p³ynów w przypadku gdy szerokoœæ poziomu nie jest podzielna przez 32)
		if (pos.x == grid_size.x - 1)
			this->fluid_tab[pos.y * grid_size.x + pos.x + 1] = true;
		//Dodatkowo je¿eli ostatnim elementem na osi Y jest p³yn, to nastêpnym te¿ jest p³yn (naprawa b³êdów dotycz¹cych ostatnich p³ynów w przypadku gdy wysokoœæ poziomu nie jest podzielna przez 32)
		if (pos.y == grid_size.y - 1)
			this->fluid_tab[(pos.y + 1) * grid_size.x + pos.x] = true;
	}
	time_optimization = clock() - time_optimization;
	//!Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu
	

	//Algorytm wzajemnej grafiki gruntu (postawiony na samym koñcu - po wszystkich dzia³aniach na gruncie)
	clock_t time_graph = clock();
	for (int i = 0; i < this->ground.size(); i++)
		this->ground[i].graphicsCustomize(sf::Vector2u(this->width, this->height), is_ground, grid_size);
	//Pozosta³e algorytmy wzajemnej grafiki
	for (int i = 0; i < this->fluid.size(); i++)
		this->fluid[i].graphicsCustomize(this->world_type, sf::Vector2u(this->width, this->height), to_fluid, grid_size);

	delete[] how_many;
	time_graph = clock() - time_graph;
	

	//GENERATOR POZIOMU (POWER-UP'Y, T£O I NPC)
	//POWER-UPS
	//Iloœc power-up'ów, które maj¹ siê pojawiæ w poziomie
	short pu_count = rand() % 3 + 2;
	//Je¿eli iloœæ power-up'ów jest wiêksza od iloœci miejsc spawnów power-up'ów, to iloœæ power-up'ów jest równa iloœci miejsc spawnu power-up'ów (pojawi¹ siê na ka¿dym spawnie)
	if (spawn_pu_pos.size() < pu_count)
		pu_count = spawn_pu_pos.size();
	//Pêtla tworzenia power-up'ów
	clock_t time_pu = clock();
	for (int i = 0; i < pu_count; i++)
	{
		unsigned short spawn = rand() % spawn_pu_pos.size();
		this->power_up.push_back(spawn_pu_pos[spawn]);
		spawn_pu_pos.erase(spawn_pu_pos.begin() + spawn);
	}
	time_pu = clock() - time_pu;


	//NPC-Y
	//Pêtla tworzenia NPC-ów
	clock_t time_npc = clock();
	for (int i = 0; i < 50; i++)
	{
		//Losowanie ID NPC-a
		int random = randomNPCID(this->world_type);
		
		//Tymczasowy NPC który bêdzie póŸniej dopisany do wektora NPC-ów (gdy zotanie dopasowany do poziomu; aktualnie nie mo¿e byæ ju¿ dopisany i zmieniany, gdy¿ algorytm sprawdza³by, czy koliduje sam ze sob¹)
		cNPC temp_npc(&(this->physics_world), this->world_type, random, this->randomPosition(416, this->width), (rand() % 2 ? DIR_LEFT : DIR_RIGHT));

		bool end = false;	//Nie przydzielono pozycji

		//Dla NPC-ów poruszaj¹cych siê po sztywnych obiektach
		if (!temp_npc.getFeatures().flying && !temp_npc.getFeatures().swimming)
		{
			//Pêtla trwa tak d³ugo a¿ NPC nie znajdzie siê dok³adanie nad powierzchni¹ sztywnego obiektu
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition(416, this->width));

				//Je¿eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je¿eli pod NPC-em znajduje siê sztywny obiekt (W przypadku NPC-ów chodz¹cych)
					if (temp_npc.isCollisionOnGrid(is_solid, grid_size) || temp_npc.isCollisionOnGrid(is_npc, grid_size))
					{
						temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y - 32));
						end = true;
					}
				}
			}
		}
		//Dla lataj¹cych NPC-ów
		else if (temp_npc.getFeatures().flying && !temp_npc.getFeatures().swimming)
		{
			//Pêtla trwa tak d³ugo a¿ NPC nie znajdzie siê odpowiednio wysoko nad powierzchni¹ sztywnego obiektu
			while (!end)
			{
				
				sf::Vector2f main_pos = this->randomPosition(416, this->width);		//G³ówna pozycja - pozycja pocz¹tkowa NPC-a
				temp_npc.setAllPositions(main_pos);

				//Je¿eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je¿eli pod NPC-em równie¿ nie znajduje siê sztywny obiekt
					if (!temp_npc.isCollisionOnGrid(is_solid, grid_size) && !temp_npc.isCollisionOnGrid(is_npc, grid_size))
					{
						for (int i = 0; i < 2; i++)		//Pêtla powtarza siê 2 razy - dok³adnie jeszcze tyle razy pod ni¹ mo¿e nie byæ gruntu
						{
							temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));
							
							if (temp_npc.isCollisionOnGrid(is_solid, grid_size) || temp_npc.isCollisionOnGrid(is_npc, grid_size))	//Je¿eli w tym miejscu znajduje siê sztywny obiekt, to mo¿e nad nim lataæ
							{
								temp_npc.setAllPositions(main_pos);
								end = true;

								break;
							}
						}
						//Je¿eli mimo tylu powtórzeñ pêtli nie znaleziono gruntu, to algorytm wyszukuje now¹ pozycjê
					}
				}
			}
		}
		//Dla p³ywaj¹cych NPC-ów
		else
		{
			//Pêtla trwa tak d³ugo a¿ NPC nie znajdzie siê w p³ynie
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition(416, this->width));
				
				//Je¿eli NPC znajduje siê w wodzie
				if (temp_npc.isCollisionOnGrid(is_fluid, grid_size))
					end = true;
			}
		}

		sf::Vector2i pos_on_grid((temp_npc.getPosition().x - temp_npc.getOrigin().x) / 32, (temp_npc.getPosition().y - temp_npc.getOrigin().y) / 32);
		is_npc[pos_on_grid.y * grid_size.x + pos_on_grid.x] = true;
		
		this->npc.push_back(temp_npc);
	}
	time_npc = clock() - time_npc;
	
	//OBIEKTY W TLE
	//Pêtla tworzenia obiektów w tle
	clock_t time_background = clock();
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
		case WORLD_DESERT:		//Bez wody, ma³o przedmiotów w gruncie
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
			else if (this->fluid.size() > 0 && random < 450)	//20% szans na obiekt w wodzie (o ile woda w ogóle istnieje)
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_FLUID_LYING;	break;}
				case 1: {type = BG_FLUID_DIVE;	break;}
				case 2: {type = BG_FLUID_FLOAT;	break;}
				}
			}
			else if (random < 850 - (fluid.empty() ? 125 : 0))	//40% szans na obiekt na gruncie, chyba ¿e woda nie istnieje, to 47.5%
			{
				switch (rand() % 3)
				{
				case 0: {type = BG_GROUND_LYING;		break;}
				case 1: {type = BG_GROUND_LEVITATING;	break;}
				case 2:
				{
					if (this->world_type == WORLD_OVERWORLD)
						type = BG_TRUNK;
					else if (this->world_type == WORLD_UNDERGROUND)	//W podziemiach nie generuj¹ siê drzewa (je¿eli bêd¹ siê mia³y generowaæ, to trzeba zmieniæ ich sposób generowania, bo w podziemiach jest mnie miejsca i drzewo mo¿e w ogóle nie znaleŸæ miejsca generowania siê)
						type = BG_CEILING;
					else
						type = BG_GROUND_LEVITATING;
					break;
				}
				}
			}
			else	//15% szans na obiekt w gruncie, chyba ¿e woda nie istnieje, to 27.5%
				type = BG_GROUND_INSIDE;

			break;
		}
		}
		//!Losowanie typu grafiki
		
		//Tymczasowy obiekt w tle który bêdzie póŸniej dopisany do wektora obiektów w tle (gdy zotanie dopasowany do poziomu; aktualnie nie mo¿e byæ ju¿ dopisany i zmieniany, gdy¿ algorytm sprawdza³by, czy koliduje sam ze sob¹)
		cBackgroundObject temp_bg_obj(this->world_type, type, this->randomPosition(0, this->width));
		
		//Sprawdzanie, czy obiekt w tle nie znajduje siê w gruncie
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

				//Je¿eli obiekt w tle nie jest zakopany w gruncie i nie znajduje siê w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_ground, grid_size) && !temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle znajduje siê grunt (bêdzie le¿a³ na gruncie)
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

				//Je¿eli obiekt w tle nie jest zakopany w gruncie i nie znajduje siê w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_ground, grid_size) && !temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle nie znajduje siê grunt (bêdzie lewitowa³ w powietrzu)
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

				//Je¿eli obiekt w tle jest zakopany w gruncie
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

				//Je¿eli obiekt w tle nie jest zakopany w sztywnym obiekcie i znajduje siê w wodzie
				if (!temp_bg_obj.isCollisionOnGrid(is_solid, grid_size) && temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle znajduje siê grunt (bêdzie le¿a³ na gruncie)
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

				//Je¿eli obiekt w tle znajduje siê w wodzie i nie znajduje siê w sztywnym obiekcie)
				if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
					{
						temp_bg_obj.move(0, -32);

						//Je¿eli wy¿ej jest woda (nie p³ywa, a znajduje siê "wewn¹trz" wody
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

				//Je¿eli obiekt w tle znajduje siê w wodzie
				if (temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je¿eli wy¿ej nie ma wody
					if (!temp_bg_obj.isCollisionOnGrid(is_fluid, grid_size))
					{
						temp_bg_obj.move(0, 26);	//Obni¿ono o nieco mniej, ¿eby obiekt "p³ywa³" na wodzie
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

				//Je¿eli obiekt w tle nie znajduje siê w sztywnym obiekcie
				if (!temp_bg_obj.isCollisionOnGrid(is_solid, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je¿eli wy¿ej jest sztywny obiekt
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
	time_background = clock() - time_background;

	//Generowanie graczy
	if (player.size() == 0)
	{
		for (int i = 0; i < number_of_players; i++)
		{
			cCharacter temp_player(&(this->physics_world), this->world_type, this->randomPosition(0, 192), i + 1);

			bool end = false;	//Nie przydzielono pozycji

			//Pêtla trwa tak d³ugo a¿ postaæ nie znajdzie siê dok³adanie nad powierzchni¹ sztywnego obiektu
			while (!end)
			{
				temp_player.setAllPositions(this->randomPosition(0, 192));

				//Je¿eli postaæ nie jest zakopany w sztywnym obiekcie
				if (!temp_player.isCollisionOnGrid(is_solid, grid_size) && !temp_player.isCollisionOnGrid(is_npc, grid_size))
				{
					temp_player.setAllPositions(sf::Vector2f(temp_player.getPosition().x, temp_player.getPosition().y + 32));

					//Je¿eli pod postaci¹ znajduje siê sztywny obiekt
					if (temp_player.isCollisionOnGrid(is_solid, grid_size) || temp_player.isCollisionOnGrid(is_npc, grid_size))
					{
						temp_player.setAllPositions(sf::Vector2f(temp_player.getPosition().x, temp_player.getPosition().y - 32));
						end = true;
					}
				}
			}

			this->player.push_back(temp_player);
		}

		for (unsigned short i = 0; i < number_of_players; i++)
			player[i].initPet();
	}
	else
	{
		for (unsigned short i = 0; i < this->player.size(); i++)
		{
			this->player[i].bodyRecreate(this->physics_world, this->world_type);	//Cia³a zawsze musz¹ siê odnowiaæ (powód: m.in. oddzia³ywanie na lód)

			bool end = false;	//Nie przydzielono pozycji

			//Pêtla trwa tak d³ugo a¿ postaæ nie znajdzie siê dok³adanie nad powierzchni¹ sztywnego obiektu
			while (!end)
			{
				this->player[i].setAllPositions(this->randomPosition(0, 192));
				
				//Je¿eli postaæ nie jest zakopana w sztywnym obiekcie
				if (!this->player[i].isCollisionOnGrid(is_solid, grid_size) && !this->player[i].isCollisionOnGrid(is_npc, grid_size))
				{
					this->player[i].setAllPositions(sf::Vector2f(this->player[i].getPosition().x, this->player[i].getPosition().y + 32));

					//Je¿eli pod postaci¹ znajduje siê sztywny obiekt
					if (this->player[i].isCollisionOnGrid(is_solid, grid_size))
					{
						this->player[i].setAllPositions(sf::Vector2f(this->player[i].getPosition().x, this->player[i].getPosition().y - 32));
						end = true;
					}
				}
			}
			
			this->player[i].rebirth();
		}
	}

	//!Generowanie graczy

	std::cout << "\n\n";
	std::cout << "Czas generowania terenu: " << time_map << "\n";
	std::cout << "Czas dostosowywania obiektow do terenu: " << time_adjust << "\n";
	std::cout << "Czas dzialania algorytmu wzajemnej grafiki gruntu: " << time_graph << "\n";
	std::cout << "Czas tworzenia tablic optymalizacyjnych: " << time_optimization << "\n";
	std::cout << "Czas rozstawiania power-up'ow na mapie: " << time_pu << "\n";
	std::cout << "Czas rozstawiania NPC na mapie: " << time_npc << "\n";
	std::cout << "Czas rozstawiania obiektow w tle na mapie: " << time_background << "\n";

	//Usuwanie zbêdnych tablic dynamicznych
	delete[] is_solid;
	delete[] is_ground;
	delete[] is_fluid;
	delete[] to_fluid;
	delete[] is_npc;
}

bool cMap::movements(sf::RenderWindow &win, sf::View &view)
{
	//SKARBY
	for (int i = this->treasure.size() - 1; i >= 0; i--)
	{
		this->treasure[i].step(this->world_type, sf::Vector2i(this->width, this->height), this->fluid_tab);

		//Wypad³ poza obszar mapy
		if (this->treasure[i].getPosition().y - this->treasure[i].getOrigin().y > this->height)
			this->treasure.erase(this->treasure.begin() + i);
	}

	//POCISKI
	for (int i = this->bullet.size() - 1; i >= 0; i--)
	{
		this->bullet[i].step(this->world_type, sf::Vector2i(this->width, this->height), fluid_tab);
		this->bullet[i].specialCollisions(&(this->physics_world), this->world_type, this->player, this->npc, this->treasure, this->bonus_block);
		
		if (this->bullet[i].isDestroyed())
		{
			this->physics_world.DestroyBody(bullet[i].getBody());
			this->bullet.erase(bullet.begin() + i);
		}
	}

	//NPC-Y
	sf::FloatRect view_rect;	//view_rect to prostok¹t, który zape³nia aktualny widok kamery. Dziêki temu mo¿na sprawdziæ, czy NPC-y znajduj¹ siê w polu widoku i maj¹ zacz¹æ siê poruszaæ.
	view_rect.left = view.getCenter().x - view.getSize().x / 2;
	view_rect.top = view.getCenter().y - view.getSize().y / 2;
	view_rect.width = view.getCenter().x + view.getSize().x / 2;
	view_rect.height = view.getCenter().y + view.getSize().y / 2;

	for (int i = this->npc.size() - 1; i >= 0; i--)
	{
		this->npc[i].step(this->world_type, sf::Vector2i(this->width, this->height), fluid_tab, view_rect);
		
		//Wyszed³ poza obszar mapy
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
		if (!this->player[i].isDead())
		{
			are_all_players_dead = false;
			
			this->player[i].control(&(this->physics_world), this->world_type, this->bullet);
			this->player[i].specialCollisions(&(this->physics_world), this->world_type, this->npc, this->power_up, this->treasure, this->fluid, this->trampoline, this->ladder, this->bonus_block);
			this->player[i].applyPhysics(this->world_type, this->fluid_tab, sf::Vector2i(this->width / 32, this->height / 32));
			this->player[i].checkIndicators(&(this->physics_world), this->world_type, this->bullet);
			this->player[i].move(win, sf::Vector2f(this->width, this->height));
			
			//Rozpoczêcie nastêpnego poziomu
			if (this->player[i].getPosition().x - this->player[i].getOrigin().x > this->width)
			{
				cShop shop(this->player);
				shop.shopMenu(win);

				menuSkillTree(win, this->player);

				this->levelGenerator(player.size(), false, true);
			}
		}
	}

	//Rozpoczêcie poziomu od nowa, je¿eli wszyscy gracze s¹ martwi
	if (are_all_players_dead)
	{
		//Zakoñczenie gry, je¿eli ¿aden z graczy nie ma wiêcej ¿yæ
		bool no_more_life = true;
		for (short i = 0; i < this->player.size(); i++)
		{
			if (this->player[i].hasLife())
			{
				no_more_life = false;
				break;
			}
		}

		if (no_more_life)
			return false;

		this->levelGenerator(player.size(), true, false);
		return true;
	}

	for (int i = npc.size() - 1; i >= 0; i--)
		if (npc[i].isDead())
			this->npc.erase(this->npc.begin() + i);
	
	this->physics_world.Step((float)1 / 60, 8, 3);
	return true;
}

void cMap::draw(sf::RenderWindow &win, sf::View &view)
{
	//Przesuwanie kamery na postaæ gracza
	sf::Vector2f view_center_point(0.0f, 0.0f);
	short how_many_player_alive = 0;
	for (int i = 0; i < this->player_number; i++)
	{
		if (!this->player[i].isDead())
		{
			view_center_point.x += this->player[i].getPosition().x;
			view_center_point.y += this->player[i].getPosition().y;
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
	//(TODO) Przesuwanie ekranu dzia³a poprawnie, jednak jeszcze nie jestem pewien jak - rozszyfrowaæ
	this->background[0].setPosition((view.getCenter().x - 400) / 1.2 + (((int)((view.getCenter().x - 400) / 1.2 / 5) / this->background[0].getTextureRect().width) * this->background[0].getTextureRect().width), (view.getCenter().y - 300) / 1.2);
	background[1].setPosition(this->background[0].getPosition().x + this->background[0].getTextureRect().width, this->background[0].getPosition().y);
	
	//Rysowanie obiektów mapy na oknie programu
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
		if (!this->player[i].isDead())
		{
			win.draw(this->player[i]);
			if (this->player[i].isPetAlive())
				win.draw(this->player[i].getPet());
		}
	for (unsigned int i = 0; i < this->background_obj.size(); i++)
		if (this->background_obj[i].front)
			this->background_obj[i].drawAllGraphics(win);
	//Statystyki graczy
	this->player[0].drawStats(win, sf::Vector2f(16, 16));
	if (this->player_number >= 2)
	{
		this->player[1].drawStats(win, sf::Vector2f(148, 16));
		if (this->player_number >= 3)
		{
			this->player[2].drawStats(win, sf::Vector2f(536, 16));
			if (this->player_number == 4)
				this->player[3].drawStats(win, sf::Vector2f(668, 16));
		}
	}

	//Numer poziomu
	sf::String lvl_no_str(L"Level:");
	
	lvl_no_str += this->level_number;
	std::string nr;
	std::stringstream ss;
	ss << this->level_number;
	nr = ss.str();
	ss.str("");
	lvl_no_str += nr;
	
	sf::Text lvl_no(lvl_no_str, font[0], 48);
	lvl_no.setOrigin(lvl_no.getGlobalBounds().width / 2, lvl_no.getGlobalBounds().height / 2);
	lvl_no.setPosition(view.getCenter().x, 48);

	win.draw(lvl_no);
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
		this->physics_world.DestroyBody(this->ground[0].getBody());		//Ca³a ziemia posiada jedno cia³o
	if (this->fluid.size() > 0)
		this->physics_world.DestroyBody(this->fluid[0].getBody());		//Ca³y p³yn posiada jedno cia³o
	
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
			if (!player[i].isDead())
				this->physics_world.DestroyBody(this->player[i].getBody());

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