#include "map.h"
#include <iostream>

cMap::cMap(eWorld world) :physics_world(b2Vec2(0.0f, 10.0f))
{
	groundsInit(&(this->physics_world));
	waterInit(&(this->physics_world));
	this->world_type = world;
	this->prev_sector.clear();
	generate();
}

void cMap::generate()
{
	//GENERATOR POZIOMU (TEREN)
	//Przypisanie tekstury do t�a (s� 2 - jedno t�o le�y za drugim (dzi�ki temu t�o mo�e si� przesuwa�))
	for (int i = 0; i < 2; i++)
		background[i].setTexture(t_background[this->world_type]);

	//Utworzenie zmiennej sektor
	cSector sector;

	//Stworzenie tablicy opisuj�cej, jak wiele w poziomie jest sektor�w o danym ID (w celach debugowania)
	std::cout << "Ilosc sektorow w tym typie siwata: " << how_many_sectors[this->world_type] << "\n\n\n";
	int *how_many = new int[how_many_sectors[this->world_type]];
	for (int i = 0; i < how_many_sectors[this->world_type]; i++)
		how_many[i] = 0;

	//Wektor zmiennych odpowiedzialnych za pozycje spawn�w bonus�w
	std::vector <sf::Vector2f> spawn_pu_pos;

	//P�tla tworzenia terenu
	clock_t time_map = clock();
	for (int i = 0; i < 50; i++)	//Ilo�� sektor�w znajduj�cych si� na mapie (GOTO zamieni� na og�ln� d�ugo�� mapy (�eby poziomy by�y podobnej d�ugo�ci))
	{
		std::cout << i << " ";
		std::string sector_id;
		//P�tla wyszukiwania pasuj�cego sektora
		do
		{
			sector.loadRandomSector(this->world_type, sector_id);
			if (i == 0)	//Pierwszy sektor zawsze pasuje
				break;
		} while (!sector.isSectorFitted(this->world_type, this->prev_sector, this->getHeight()));

		int s_id = atoi((char*)sector_id.c_str());
		std::cout << s_id << "\n";
		how_many[s_id - 1]++;

		//Przypisanie sektora poprzedniemu sektorowi - dzi�ki temu mo�na b�dzie znowu wyszukiwa� pasuj�cy sektor
		this->prev_sector = sector;

		//Dla �wiata podziemnego generator tworzy ponad sektorem warstwy gruntu
		if (this->world_type == WORLD_UNDERGROUND)
			for (unsigned int i = 0; i < ceil((float)this->height / 32) - sector.getHeight(); i++)
				for (unsigned int j = 0; j < sector.getWidth(); j++)
				{
					//std::cout << j << ":" << i << "\n";
					this->ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16), this->world_type));//sector.setObject(j, i, OBJECT_GROUND);
				}
		//!Dla �wiata podziemnego generator tworzy ponad sektorem warstwy gruntu

		//Je�eli sektor jest wy�szy od aktualnej wysoko�ci poziomu, to ca�y poziom staje si� wy�szy (kamera mo�e pod��a� wy�ej)
		if (sector.getHeight() * 32 + 64 > this->height)
			this->height = sector.getHeight() * 32 + 64;

		int to_down = g_height - sector.getHeight() * 32;	//Przyrownanie calego terenu do podloza mapy (wysokosc mapy - wysokosc sektora)
		for (int i = 0; i < sector.getHeight(); i++)
			for (int j = 0; j < sector.getWidth(); j++)
			{
				switch (sector.getObject(j, i))
				{
				case eObjType::OBJECT_GROUND: {ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), this->world_type));	break;}
				case eObjType::OBJECT_BLOCK: {block.push_back(cBlock(&(this->physics_world), t_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_BONUS_BLOCK: {bonus_block.push_back(cBonusBlock(&(this->physics_world), t_bonus_block[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_WATER: {water.push_back(cWater(t_object[1], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_TREASURE: {treasure.push_back(cTreasure(&(this->physics_world), sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				case eObjType::OBJECT_TRAMPOLINE: {trampoline.push_back(cTrampoline(&(this->physics_world), 1, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), 5.0f));	break;}
				case eObjType::OBJECT_POWER_UP: {spawn_pu_pos.push_back(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down));	break;}
				case eObjType::OBJECT_LADDER: {ladder.push_back(cLadder(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;}
				}
			}

		//Zwi�kszanie szeroko�ci poziomu
		x_generate += sector.getWidth() * 32;
		this->width = x_generate;
	}
	time_map = clock() - time_map;
	

	//Wy�wietlanie na ekranie jak wiele jest sektor�w o danym ID (W celach debugowania)
	std::cout << "\n\n\n";
	for (int i = 0; i < how_many_sectors[this->world_type]; i++)
		std::cout << i + 1 << " = " << how_many[i] << "\n";


	//Dostosowywanie obiekt�w mapy do ustawie� mapy
	clock_t time_adjust = clock();
	for (unsigned int i = 0; i < ground.size(); i++)
		this->ground[i].adjustObjectToLevel(this->height);
	for (unsigned int i = 0; i < water.size(); i++)
		this->water[i].adjustObjectToLevel(this->height);
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
	time_adjust = clock() - time_adjust;
	//!Dostosowywanie obiekt�w mapy do ustawie� mapy
	

	//Zmiany w terenie ze wzgl�du na typ poziomu
	/*switch (this->world_type)
	{
	
	DLA �WIATA NAZIEMNEGO:
	-Zwi�kszanie wysoko�ci poziomu (mo�na chodzi� po najwy�szych sektorach
	
	case WORLD_OVERWORLD:
	{
		this->height += 3 * 32;
		break;
	}
	
	DLA �WIATA PODZIEMNEGO:
	-Tworzenie gruntu ponad sektorami
	
	case WORLD_UNDERGROUND:
	{
		for (unsigned int i = 0; i < this->height)
		{

		}
		break;
	}
	}*/

	//!Zmiany w terenie ze wzgl�du na typ poziomu


	//Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu
	clock_t time_optimization = clock();
	sf::Vector2i grid_size(this->width / 32, this->height / 32);	//Wymiary siatki (wymiary poziomu podzielone przez 32)
	bool *is_solid = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� sztywny obiekt (grunt, blok, ...) - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_ground = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� grunt - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_water = new bool[grid_size.x * grid_size.y];	//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� woda - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	bool *is_npc = new bool[grid_size.x * grid_size.y];		//Tablica odpowiadaj�ca za to, czy w danym punkcie znajduje si� npc - dzi�ki temu mo�na zoptymalizowa� generowanie obiekt�w w poziomie
	for (unsigned int i = 0; i < grid_size.y; i++)
		for (unsigned int j = 0; j < grid_size.x; j++)
		{
			is_solid[i * grid_size.x + j] = false;
			is_ground[i * grid_size.x + j] = false;
			is_water[i * grid_size.x + j] = false;
			is_npc[i * grid_size.x + j] = false;
		}

	for (unsigned int i = 0; i < this->ground.size(); i++)
	{
		sf::Vector2i pos = this->ground[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
		is_ground[pos.y * grid_size.x + pos.x] = true;
	}
	for (unsigned int i = 0; i < this->block.size(); i++)
	{
		sf::Vector2i pos = this->block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
	}
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
	{
		sf::Vector2i pos = this->bonus_block[i].posOnGrid(sf::Vector2i(32, 32));
		is_solid[pos.y * grid_size.x + pos.x] = true;
	}
	for (unsigned int i = 0; i < this->water.size(); i++)
	{
		sf::Vector2i pos = this->water[i].posOnGrid(sf::Vector2i(32, 32));
		is_water[pos.y * grid_size.x + pos.x] = true;
	}

	/*for (unsigned int i = 0; i < grid_size.y; i++)
	{
	for (unsigned int j = 0; j < 32; j++)
	{
	if (is_ground[i * grid_size.x + j])
	std::cout << '1';
	else if (is_water[i * grid_size.x + j])
	std::cout << '4';
	else if (is_solid[i * grid_size.x + j])
	std::cout << '2';
	else
	std::cout << '0';
	}
	std::cout << "\n";
	}*/

	time_optimization = clock() - time_optimization;
	//!Tworzenie tablic odpowiedzialnych za optymalizacje generowania poziomu
	

	//Algorytm wzajemnej grafiki gruntu (postawiony na samym ko�cu - po wszystkich dzia�aniach na gruncie)
	clock_t time_graph = clock();
	for (int i = 0; i < this->ground.size(); i++)
		this->ground[i].graphicsCustomize(sf::Vector2u(this->width, this->height), is_ground, grid_size);
	//Pozosta�e algorytmy wzajemnej grafiki
	for (int i = 0; i < this->water.size(); i++)
		this->water[i].graphicsCustomize(sf::Vector2u(this->width, this->height), is_water, grid_size);

	delete how_many;
	time_graph = clock() - time_graph;
	

	//GENERATOR POZIOMU (POWER-UP'Y, T�O I NPC)
	//POWER-UPS
	//Ilo�c power-up'�w, kt�re maj� si� pojawi� w poziomie
	short pu_count = rand() % 3 + 2;
	//Je�eli ilo�� power-up'�w jest wi�ksza od ilo�ci miejsc spawn�w power-up'�w, to ilo�� power-up'�w jest r�wna ilo�ci miejsc spawnu power-up'�w (pojawi� si� na ka�dym spawnie)
	if (spawn_pu_pos.size() < pu_count)
		pu_count = spawn_pu_pos.size();
	//P�tla tworzenia power-up'�w
	clock_t time_pu = clock();
	for (int i = 0; i < pu_count; i++)
	{
		unsigned short spawn = rand() % spawn_pu_pos.size();
		this->power_up.push_back(spawn_pu_pos[spawn]);
		spawn_pu_pos.erase(spawn_pu_pos.begin() + spawn);
	}
	time_pu = clock() - time_pu;


	//NPC-Y
	//P�tla tworzenia NPC-�w
	clock_t time_npc = clock();
	for (int i = 0; i < 50; i++)
	{
		//Tymczasowy NPC kt�ry b�dzie p�niej dopisany do wektora NPC-�w (gdy zotanie dopasowany do poziomu; aktualnie nie mo�e by� ju� dopisany i zmieniany, gdy� algorytm sprawdza�by, czy koliduje sam ze sob�)
		cNPC temp_npc(&(this->physics_world), rand() % 3 + 1, this->randomPosition(), (rand() % 2 ? DIR_LEFT : DIR_RIGHT));

		bool end = false;	//Nie przydzielono pozycji

		//Dla NPC-�w poruszaj�cych si� po sztywnych obiektach
		if (!temp_npc.getFeatures().flying)
		{
			//P�tla trwa tak d�ugo a� NPC nie znajdzie si� dok�adanie nad powierzchni� sztywnego obiektu
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition());

				//Je�eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isSolidCollision(is_solid, is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je�eli pod NPC-em znajduje si� sztywny obiekt (W przypadku NPC-�w chodz�cych)
					if (temp_npc.isSolidCollision(is_solid, is_npc, grid_size))
					{
						temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y - 32));
						end = true;
					}
				}
			}
		}
		//Dla lataj�cych NPC-�w
		else
		{
			//P�tla trwa tak d�ugo a� NPC nie znajdzie si� odpowiednio wysoko nad powierzchni� sztywnego obiektu
			while (!end)
			{
				temp_npc.setAllPositions(this->randomPosition());
				sf::Vector2f main_pos = temp_npc.getPosition();		//G��wna pozycja - pozycja pocz�tkowa NPC-a

				//Je�eli NPC nie jest zakopany w sztywnym obiekcie
				if (!temp_npc.isSolidCollision(is_solid, is_npc, grid_size))
				{
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));

					//Je�eli pod NPC-em r�wnie� nie znajduje si� sztywny obiekt
					if (!temp_npc.isSolidCollision(is_solid, is_npc, grid_size))
					{
						for (int i = 0; i < 2; i++)		//P�tla powtarza si� 2 razy - dok�adnie jeszcze tyle razy pod ni� mo�e nie by� gruntu
						{
							temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));
							
							if (temp_npc.isSolidCollision(is_solid, is_npc, grid_size))	//Je�eli w tym miejscu znajduje si� sztywny obiekt, to mo�e nad nim lata�
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

		sf::Vector2i pos_on_grid((temp_npc.getPosition().x - temp_npc.getOrigin().x) / 32, (temp_npc.getPosition().y - temp_npc.getOrigin().y) / 32);
		is_npc[pos_on_grid.y * grid_size.x + pos_on_grid.x] = true;

		this->npc.push_back(temp_npc);
	}
	time_npc = clock() - time_npc;
	

	//OBIEKTY W TLE
	//P�tla tworzenia obiekt�w w tle
	clock_t time_background = clock();
	for (int i = 0; i < 100; i++)
	{
		eBackgroundType type;
		
		//Losowanie typu grafiki
		int random = rand() % 1001;	//0,0% - 100,0%
			
		if (random < 250)	//25% szans na obiekt w powietrzu
			type = BG_FLYING;
		else if (this->water.size() > 0 && random < 450)	//20% szans na obiekt w wodzie (o ile woda w og�le istnieje)
		{
			switch (rand() % 3)
			{
			case 0: {type = BG_WATER_LYING;	break;}
			case 1: {type = BG_WATER_DIVE;	break;}
			case 2: {type = BG_WATER_FLOAT;	break;}
			}
		}
		else if (random < 850 - (water.empty() ? 125 : 0))	//40% szans na obiekt na gruncie, chyba �e woda nie istnieje, to 47.5%
		{
			switch (rand () %3)
			{
			case 0: {type = BG_GROUND_LYING;		break;}
			case 1: {type = BG_GROUND_LEVITATING;	break;}
			case 2:
			{
				if (this->world_type == WORLD_OVERWORLD)
					type = BG_TRUNK;
				else if (this->world_type == WORLD_UNDERGROUND)	//W podziemiach nie generuj� si� drzewa (je�eli b�d� si� mia�y generowa�, to trzeba zmieni� ich spos�b generowania, bo w podziemiach jest mnie miejsca i drzewo mo�e w og�le nie znale�� miejsca generowania si�)
					type = BG_CEILING;
				break;
			}
			}
		}
		else	//15% szans na obiekt w gruncie, chyba �e woda nie istnieje, to 27.5%
			type = BG_GROUND_INSIDE;
		//!Losowanie typu grafiki
		
		//Tymczasowy obiekt w tle kt�ry b�dzie p�niej dopisany do wektora obiekt�w w tle (gdy zotanie dopasowany do poziomu; aktualnie nie mo�e by� ju� dopisany i zmieniany, gdy� algorytm sprawdza�by, czy koliduje sam ze sob�)
		cBackgroundObject temp_bg_obj(this->world_type, type, this->randomPosition());
		
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
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle nie jest zakopany w gruncie i nie znajduje si� w wodzie
				if (!temp_bg_obj.isGroundCollision(is_ground, grid_size) && !temp_bg_obj.isWaterCollision(is_water, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle znajduje si� grunt (b�dzie le�a� na gruncie)
					if (temp_bg_obj.isGroundCollision(is_ground, grid_size))
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
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle nie jest zakopany w gruncie i nie znajduje si� w wodzie
				if (!temp_bg_obj.isGroundCollision(is_ground, grid_size) && !temp_bg_obj.isWaterCollision(is_water, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle nie znajduje si� grunt (b�dzie lewitowa� w powietrzu)
					if (!temp_bg_obj.isGroundCollision(is_ground, grid_size))
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
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle jest zakopany w gruncie
				if (temp_bg_obj.isGroundCollision(is_ground, grid_size))
					end = true;
			}
			break;
		}
		case BG_WATER_LYING:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle nie jest zakopany w gruncie i znajduje si� w wodzie
				if (!temp_bg_obj.isGroundCollision(is_ground, grid_size) && temp_bg_obj.isWaterCollision(is_water, grid_size))
				{
					temp_bg_obj.move(0, 32);
					//Je�eli pod obiektem w tle znajduje si� grunt (b�dzie le�a� na gruncie)
					if (temp_bg_obj.isGroundCollision(is_ground, grid_size))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case BG_WATER_DIVE:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle znajduje si� w wodzie
				if (temp_bg_obj.isWaterCollision(is_water, grid_size))
				{
					if (temp_bg_obj.isWaterCollision(is_water, grid_size))
					{
						temp_bg_obj.move(0, -32);

						//Je�eli wy�ej jest woda (nie p�ywa, a znajduje si� "wewn�trz" wody
						if (temp_bg_obj.isWaterCollision(is_water, grid_size))
						{
							temp_bg_obj.move(0, 32);
							end = true;
						}
					}
				}
			}
			break;
		}
		case BG_WATER_FLOAT:
		{
			bool end = false;
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle znajduje si� w wodzie
				if (temp_bg_obj.isWaterCollision(is_water, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je�eli wy�ej nie ma wody
					if (!temp_bg_obj.isWaterCollision(is_water, grid_size))
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
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je�eli obiekt w tle nie znajduje si� w sztywnym obiekcie
				if (!temp_bg_obj.isGroundCollision(is_solid, grid_size))
				{
					temp_bg_obj.move(0, -32);

					//Je�eli wy�ej jest sztywny obiekt
					if (temp_bg_obj.isGroundCollision(is_solid, grid_size))
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

	std::cout << "\n\n";
	std::cout << "Czas generowania terenu: " << time_map << "\n";
	std::cout << "Czas dostosowywania obiektow do terenu: " << time_adjust << "\n";
	std::cout << "Czas dzialania algorytmu wzajemnej grafiki gruntu: " << time_graph << "\n";
	std::cout << "Czas tworzenia tablic optymalizacyjnych: " << time_optimization << "\n";
	std::cout << "Czas rozstawiania power-up'ow na mapie: " << time_pu << "\n";
	std::cout << "Czas rozstawiania NPC na mapie: " << time_npc << "\n";
	std::cout << "Czas rozstawiania obiektow w tle na mapie: " << time_background << "\n";
}

void cMap::movements(sf::View &view)
{
	static int abc = 0;
	abc++;
	if (abc == 100)
	{
		abc = 0;
		//for (int i = 0; i < this->bonus_block.size(); i++)
		if (bonus_block.size() > 0)
		{
			this->bonus_block[0].dropTreasures(&(this->physics_world), this->treasure);
			this->physics_world.DestroyBody(this->bonus_block[0].getBody());
			bonus_block.erase(bonus_block.begin());
		}
	}

	for (unsigned int i = 0; i < this->treasure.size(); i++)
		this->treasure[i].step();

	sf::FloatRect view_rect;	//view_rect to prostok�t, kt�ry zape�nia aktualny widok kamery. Dzi�ki temu mo�na sprawdzi�, czy NPC-y znajduj� si� w polu widoku i maj� zacz�� si� porusza�.
	view_rect.left = view.getCenter().x - view.getSize().x / 2;
	view_rect.top = view.getCenter().y - view.getSize().y / 2;
	view_rect.width = view.getCenter().x + view.getSize().x / 2;
	view_rect.height = view.getCenter().y + view.getSize().y / 2;

	for (unsigned int i = 0; i < this->npc.size(); i++)
		this->npc[i].step(view_rect);

	this->physics_world.Step((float)1 / 60, 8, 3);
}

void cMap::draw(sf::RenderWindow &win, sf::View &view)
{
	//Przesuwanie ekranu
	//(TODO) Przesuwanie ekranu dzia�a poprawnie, jednak jeszcze nie jestem pewien jak - rozszyfrowa�
	this->background[0].setPosition((view.getCenter().x - 400) / 1.2 + (((int)((view.getCenter().x - 400) / 1.2 / 5) / this->background[0].getTextureRect().width) * this->background[0].getTextureRect().width), (view.getCenter().y - 300) / 1.2);
	background[1].setPosition(this->background[0].getPosition().x + this->background[0].getTextureRect().width, this->background[0].getPosition().y);
	
	//Rysowanie obiekt�w mapy na oknie programu
	for (unsigned short i = 0; i < 2; i++)
		win.draw(this->background[i]);
	for (unsigned int i = 0; i < this->water.size(); i++)
		win.draw(this->water[i]);
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
	for (unsigned int i = 0; i < this->npc.size(); i++)
		win.draw(this->npc[i]);
	for (unsigned int i = 0; i < this->background_obj.size(); i++)
		if (this->background_obj[i].front)
			this->background_obj[i].drawAllGraphics(win);
}

unsigned int cMap::getWidth()
{
	return this->width;
}

unsigned int cMap::getHeight()
{
	return this->height;
}

sf::Vector2f cMap::randomPosition()
{
	return sf::Vector2f(this->width - (rand() % (this->width / 32) * 32 + 16), this->height - (rand() % (this->height / 32) * 32 + 16));
}

void cMap::destroy()
{
	for (int i = 0; i < this->physics_world.GetBodyCount(); i++)
		this->physics_world.DestroyBody(this->physics_world.GetBodyList());
	for (int i = 0; i < this->physics_world.GetJointCount(); i++)
		this->physics_world.DestroyBody(this->physics_world.GetBodyList());

	this->ground.clear();
	this->water.clear();
	this->block.clear();
	this->bonus_block.clear();
	this->npc.clear();
	this->treasure.clear();
	this->background_obj.clear();
	this->trampoline.clear();
}