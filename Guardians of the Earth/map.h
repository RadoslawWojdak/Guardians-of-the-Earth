#ifndef map_h
#define map_h

#include <vector>
#include <sstream>
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
#include "fonts.h"
#include "bullet.h"
#include "shop.h"
#include "menu.h"
#include "knight.h"
#include "archer.h"
#include "spy.h"
#include "sorceress.h"
#include "scoreboard.h"

/*
Skr�ty (skr�t + s = liczba mnoga):
bb - bonusowy blok
*/

class cMap
{
	eWorld world_type;
	b2World physics_world;

	unsigned int width, height = 600;
	unsigned int x_generate = 0;		//aktualne miejsce w ktorym ma sie wygenerowac nastepny sektor (aktualizuje sie po stworzeniu sektora)
	cSector prev_sector;
	unsigned int level_number;
	short player_number;

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
	std::vector <cCharacter*> player;
	std::vector <cBullet> bullet;
	//!Obiekty na mapie

	//Zapasowe obiekty mapy (obiekty u�ywane po �mierci wszystkich graczy do cofni�cia mapy do stanu sprzed jej rozpocz�cia)
	std::vector <cSector> reserve_sector;
	//!Zapasowe obiekty mapy

	unsigned short experience_countdown;	//Czas na przej�cie poziomu (w zale�no�ci ile go zostanie, tyle do�wiadczenia dostan� postacie graczy) - w klatkach (p�niej podzielony na sekundy)
	unsigned int initial_bbs_size;
	bool golden_bb_created;

	bool *fluid_tab;	//Tablica p�yn�w (Dopasowana do siatki 32x32 - sprawdza, czy w danym polu siatki znajduje si� p�yn)

public:
	cMap(sf::RenderWindow &win, cProfile &profile, eWorld world, short number_of_players, eCharacter character[], bool *modulators);				//Tworzenie mapy za pomoca funkcji generate()
	
	void levelGenerator(sf::RenderWindow &win, cProfile &profile, short number_of_players, bool *modulators, bool refresh, bool next_level, eCharacter character[] = {});	//Generator ca�ego poziomu;	refresh - czy poziom ma by� od�wie�ony? (Je�eli tak to korzysta z gotowych sektor�w. W przeciwnym razie tworzy poziom od podstaw.)
	
	bool movements(sf::RenderWindow &win, sf::View &view, bool *modulators, cScoreboard &scoreboard, cProfile &profile);	//Ruch �wiata (Box2D + ewentualne dodatkowe) (w przypdaku �mierci wsyzstkich graczy zwraca FALSE)
	void draw(sf::RenderWindow &win, sf::View &view);	//Wy�wietlanie mapy na ekran

	unsigned int getWidth();
	unsigned int getHeight();

	sf::Vector2f randomPosition(unsigned int min_x, unsigned int max_x);

	void destroy(bool destroy_players);
};

#endif // !map_h