#ifndef map_h
#define map_h

#include <vector>
#include "graphics.h"
#include "sector.h"
#include "block.h"
#include "bonusblock.h"
#include "ground.h"
#include "treasure.h"
#include "npc.h"
#include "backgroundobject.h"
#include "trampoline.h"
#include "water.h"

class cMap
{
	eWorld world_type;
	b2World physics_world;

	unsigned int width, height = 600;
	unsigned int x_generate = 0;		//aktualne miejsce w ktorym ma sie wygenerowac nastepny sektor (aktualizuje sie po stworzeniu sektora)
	cSector prev_sector;

	sf::Sprite background[2];			//Tlo mapy (dziêki dwóm takim samym t³om, t³o mo¿e siê przesuwaæ)

	std::vector <cGround> ground;			//WEKTOR KLASY GROUND
	std::vector <cWater> water;				//WEKTOR KLASY WATER
	std::vector <cBlock> block;				//WEKTOR KLASY BLOCK
	std::vector <cBonusBlock> bonus_block;	//WEKTOR KLASY BLOCK - DLA BONUSOWYCH BLOKOW (Z ZAWARTOSCIA)
	std::vector <cNPC> npc;					//WEKTOR KLASY NPC
	std::vector <cTreasure> treasure;		//WEKTOR KLASY TREASURE
	std::vector <cBackgroundObject> background_obj;
	std::vector <cTrampoline> trampoline;

public:
	cMap(eWorld world);				//Tworzenie mapy za pomoca funkcji generate()
	void generate();	//Generowanie mapy
	void movements(sf::View &view);	//Ruch œwiata (Box2D + ewentualne dodatkowe)
	void draw(sf::RenderWindow &win, sf::View &view);	//Wyœwietlanie mapy na ekran

	unsigned int getWidth();
	unsigned int getHeight();

	sf::Vector2f randomPosition();

	void destroy();
};

#endif // !map_h
