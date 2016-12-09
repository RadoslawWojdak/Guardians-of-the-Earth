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
	//Przypisanie tekstury do t³a (s¹ 2 - jedno t³o le¿y za drugim (dziêki temu t³o mo¿e siê przesuwaæ))
	for (int i = 0; i < 2; i++)
		background[i].setTexture(t_background[this->world_type][0]);

	//Utworzenie zmiennej sektor
	cSector sector;
	
	//Stworzenie tablicy opisuj¹cej, jak wiele w poziomie jest sektorów o danym ID (w celach debugowania)
	std::cout << "Ilosc sektorow w tym typie siwata: " << how_many_sectors[this->world_type] << "\n\n\n";
	int *how_many = new int[how_many_sectors[this->world_type]];
	for (int i = 0; i < how_many_sectors[this->world_type]; i++)
		how_many[i] = 0;
	
	//Pêtla tworzenia terenu
	clock_t time_map = clock();
	for (int i = 0; i < 75; i++)	//Iloœæ sektorów znajduj¹cych siê na mapie (GOTO zamieniæ na ogóln¹ d³ugoœæ mapy (¿eby poziomy by³y podobnej d³ugoœci))
	{
		std::cout << i << " ";
		std::string sector_id;
		//Pêtla wyszukiwania pasuj¹cego sektora
		do
		{
			sector.loadRandomSector(this->world_type, sector_id);
			if (i == 0)	//Pierwszy sektor zawsze pasuje
				break;
		} while (!sector.isSectorFitted(this->prev_sector, this->getHeight()));
		int s_id = atoi((char*)sector_id.c_str());
		std::cout << s_id << "\n";
		how_many[s_id - 1]++;

		//Przypisanie sektora poprzedniemu sektorowi - dziêki temu mo¿na bêdzie znowu wyszukiwaæ pasuj¹cy sektor
		this->prev_sector = sector;

		//Je¿eli sektor jest wy¿szy od aktualnej wysokoœci poziomu, to ca³y poziom staje siê wy¿szy (kamera mo¿e pod¹¿aæ wy¿ej)
		if (sector.getHeight() * 32 + 64 > this->height)
			this->height = sector.getHeight() * 32 + 64;

		int to_down = g_height - sector.getHeight() * 32;	//Przyrownanie calego terenu do podloza mapy (wysokosc mapy - wysokosc sektora)
		for (int i = 0; i < sector.getHeight(); i++)
			for (int j = 0; j < sector.getWidth(); j++)
			{
				switch (sector.getObject(j, i))
				{
				case eObjType::ground:		ground.push_back(cGround(sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), this->world_type));	break;
				case eObjType::block:		block.push_back(cBlock(&(this->physics_world), t_block_overworld[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;
				case eObjType::bonus_block:	bonus_block.push_back(cBonusBlock(&(this->physics_world), t_bonus_block_overworld[0], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;
				case eObjType::water:		water.push_back(cWater(t_object[1], sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;
				case eObjType::treasure:	treasure.push_back(cTreasure(&(this->physics_world), sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down)));	break;
				case eObjType::trampoline:	trampoline.push_back(cTrampoline(&(this->physics_world), 1, sf::Vector2f(x_generate + j * 32 + 16, i * 32 + 16 + to_down), 5.0f));	break;
				}
			}
		//Zwiêkszanie szerokoœci poziomu
		x_generate += sector.getWidth() * 32;
		this->width = x_generate;
	}
	time_map = clock() - time_map;

	//Wyœwietlanie na ekranie jak wiele jest sektorów o danym ID (W celach debugowania)
	std::cout << "\n\n\n";
	for (int i = 0; i < how_many_sectors[this->world_type]; i++)
		std::cout << i + 1 << " = " << how_many[i] << "\n";
	
	clock_t time_adjust = clock();
	//Dostosowywanie obiektów mapy do ustawieñ mapy
	for (int i = 0; i < ground.size(); i++)
		ground[i].adjustObjectToLevel(this->height);
	for (int i = 0; i < block.size(); i++)
		block[i].adjustObjectToLevel(this->height);
	for (int i = 0; i < bonus_block.size(); i++)
		bonus_block[i].adjustObjectToLevel(this->height);
	for (int i = 0; i < treasure.size(); i++)
		treasure[i].adjustObjectToLevel(this->height);
	time_adjust = clock() - time_adjust;

	//Algorytm wzajemnej grafiki gruntu (postawiony na samym koñcu - po wszystkich dzia³aniach na gruncie)
	clock_t time_graph = clock();
	for (int i = 0; i < this->ground.size(); i++)
		this->ground[i].graphicsCustomize(sf::Vector2u(this->width, this->height), this->ground);
	for (int i = 0; i < this->water.size(); i++)
		this->water[i].graphicsCustomize(sf::Vector2u(this->width, this->height), this->water);

	delete how_many;
	time_graph = clock() - time_graph;

	
	//GENERATOR POZIOMU (T£O I NPC)
	//Pêtla tworzenia NPC-ów
	clock_t time_npc = clock();
	for (int i = 0; i < 50; i++)
	{
		//Tymczasowy NPC który bêdzie póŸniej dopisany do wektora NPC-ów (gdy zotanie dopasowany do poziomu; aktualnie nie mo¿e byæ ju¿ dopisany i zmieniany, gdy¿ algorytm sprawdza³by, czy koliduje sam ze sob¹)
		cNPC temp_npc(&(this->physics_world), rand() % 1 + 1, this->randomPosition(), (rand() % 2 ? DIR_LEFT : DIR_RIGHT));

		//Sprawdzanie, czy NPC nie utkn¹³ w jakimœ sztywnym obiekcie
		bool end = false;	//Nie przydzielono pozycji
		while (!end)
		{
			temp_npc.setAllPositions(this->randomPosition());
			
			//Je¿eli NPC nie jest zakopany w sztywnym obiekcie
			if (!temp_npc.isSolidCollision(this->ground, this->block, this->bonus_block, this->npc))
			{
				temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y + 32));
				//temp_npc.move(0, 32);
				//Je¿eli pod NPC-em znajduje siê sztywny obiekt (W przypadku NPC-ów chodz¹cych)
				if (temp_npc.isSolidCollision(this->ground, this->block, this->bonus_block, this->npc))
				{
					//exit(0);
					temp_npc.setAllPositions(sf::Vector2f(temp_npc.getPosition().x, temp_npc.getPosition().y - 32));
					//temp_npc.move(0, -32);
					end = true;
				}
			}
		}

		this->npc.push_back(temp_npc);
	}
	time_npc = clock() - time_npc;
	
	//Pêtla tworzenia obiektów w tle
	clock_t time_background = clock();
	for (int i = 0; i < 0; i++)
	{
		eType type;
		bool end = false;
		while (!end)
		{
			end = true;
			type = (eType)(rand() % 5);
			if (this->water.size() <= 0 && (type == TYPE_WATER || type == TYPE_GROUND_WATER))
				end = false;
		}
		//Tymczasowy obiekt w tle który bêdzie póŸniej dopisany do wektora obiektów w tle (gdy zotanie dopasowany do poziomu; aktualnie nie mo¿e byæ ju¿ dopisany i zmieniany, gdy¿ algorytm sprawdza³by, czy koliduje sam ze sob¹)
		cBackgroundObject temp_bg_obj(type, this->randomPosition());

		//Sprawdzanie, czy obiekt w tle nie znajduje siê w gruncie
		switch (type)
		{
		case TYPE_GROUND:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je¿eli obiekt w tle nie jest zakopany w gruncie i nie znajduje siê w wodzie
				if (!temp_bg_obj.isGroundCollision(this->ground) && !temp_bg_obj.isWaterCollision(this->water))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle znajduje siê grunt (bêdzie le¿a³ na gruncie)
					if (temp_bg_obj.isGroundCollision(this->ground))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case TYPE_AIR:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je¿eli obiekt w tle nie jest zakopany w gruncie i nie znajduje siê w wodzie
				if (!temp_bg_obj.isGroundCollision(this->ground) && !temp_bg_obj.isWaterCollision(this->water))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle nie znajduje siê grunt (bêdzie lewitowa³ w powietrzu)
					if (!temp_bg_obj.isGroundCollision(this->ground))
					{
						temp_bg_obj.move(0, -32);
						end = true;
					}
				}
			}
			break;
		}
		case TYPE_WATER:	//TODO stworzyæ wodê
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je¿eli obiekt w tle jest zakopany w gruncie
				if (temp_bg_obj.isWaterCollision(this->water))
					end = true;
			}
			break;
		}
		case TYPE_SOIL:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je¿eli obiekt w tle jest zakopany w gruncie
				if (temp_bg_obj.isGroundCollision(this->ground))
					end = true;
			}
			break;
		}
		case TYPE_GROUND_WATER:
		{
			bool end = false;	//Nie przydzielono pozycji
			while (!end)
			{
				temp_bg_obj.setAllPosition(this->randomPosition());

				//Je¿eli obiekt w tle nie jest zakopany w gruncie i znajduje siê w wodzie
				if (!temp_bg_obj.isGroundCollision(this->ground) && temp_bg_obj.isWaterCollision(this->water))
				{
					temp_bg_obj.move(0, 32);
					//Je¿eli pod obiektem w tle znajduje siê grunt (bêdzie le¿a³ na gruncie)
					if (temp_bg_obj.isGroundCollision(this->ground))
					{
						temp_bg_obj.move(0, -32);
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

	sf::FloatRect view_rect;	//view_rect to prostok¹t, który zape³nia aktualny widok kamery. Dziêki temu mo¿na sprawdziæ, czy NPC-y znajduj¹ siê w polu widoku i maj¹ zacz¹æ siê poruszaæ.
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
	//(TODO) Przesuwanie ekranu dzia³a poprawnie, jednak jeszcze nie jestem pewien jak - rozszyfrowaæ
	this->background[0].setPosition((view.getCenter().x - 400) / 1.2 + (((int)((view.getCenter().x - 400) / 1.2 / 5) / this->background[0].getTextureRect().width) * this->background[0].getTextureRect().width), (view.getCenter().y - 300) / 1.2);
	background[1].setPosition(this->background[0].getPosition().x + this->background[0].getTextureRect().width, this->background[0].getPosition().y);
	
	//Rysowanie obiektów mapy na oknie programu
	for (unsigned short i = 0; i < 2; i++)
		win.draw(this->background[i]);
	for (unsigned int i = 0; i < this->water.size(); i++)
		win.draw(this->water[i]);
	for (unsigned int i = 0; i < this->ground.size(); i++)
		win.draw(this->ground[i]);
	for (unsigned int i = 0; i < this->background_obj.size(); i++)
		this->background_obj[i].drawAllGraphics(win);
	for (unsigned int i = 0; i < this->bonus_block.size(); i++)
		win.draw(this->bonus_block[i]);
	for (unsigned int i = 0; i < this->block.size(); i++)
		win.draw(this->block[i]);
	for (unsigned int i = 0; i < this->trampoline.size(); i++)
		win.draw(this->trampoline[i]);
	for (unsigned int i = 0; i < this->treasure.size(); i++)
		win.draw(this->treasure[i]);
	for (unsigned int i = 0; i < this->npc.size(); i++)
		win.draw(this->npc[i]);
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