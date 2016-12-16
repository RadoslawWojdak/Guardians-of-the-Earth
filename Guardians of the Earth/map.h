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
#include "fluid.h"
#include "powerup.h"
#include "ladder.h"
#include "character.h"

class cMap
{
	eWorld world_type;
	b2World physics_world;

	unsigned int width, height = 600;
	unsigned int x_generate = 0;		//aktualne miejsce w ktorym ma sie wygenerowac nastepny sektor (aktualizuje sie po stworzeniu sektora)
	cSector prev_sector;

	sf::Sprite background[2];			//Tlo mapy (dzi�ki dw�m takim samym t�om, t�o mo�e si� przesuwa�)

	//Obiekty na mapie
	std::vector <cGround> ground;
	std::vector <cFluid> fluid;
	std::vector <cBlock> block;
	std::vector <cBonusBlock> bonus_block;
	std::vector <cNPC> npc;
	std::vector <cTreasure> treasure;
	std::vector <cBackgroundObject> background_obj;
	std::vector <cTrampoline> trampoline;
	std::vector <cPowerUp> power_up;
	std::vector <cLadder> ladder;
	std::vector <cCharacter> player;
	//!Obiekty na mapie

	bool *fluid_tab;	//Tablica p�yn�w (Dopasowana do siatki 32x32 - sprawdza, czy w danym polu siatki znajduje si� p�yn)

public:
	cMap(eWorld world, short number_of_players);				//Tworzenie mapy za pomoca funkcji generate()
	void generate(short number_of_players);	//Generowanie mapy
	void movements(sf::View &view);	//Ruch �wiata (Box2D + ewentualne dodatkowe)
	void draw(sf::RenderWindow &win, sf::View &view);	//Wy�wietlanie mapy na ekran

	unsigned int getWidth();
	unsigned int getHeight();

	sf::Vector2f randomPosition(/*unsigned int min_x, unsigned int max_x*/);

	void destroy();
};

#endif // !map_h