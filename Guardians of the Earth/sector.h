#ifndef sector_h
#define sector_h

#include "SFML/Graphics.hpp"	//Tylko dla wyœwietlania b³êdów!
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include "enums.h"
#include "dialogs.h"

extern unsigned int how_many_sectors[7];	//Jak wiele jest sektorow danego typu

bool howManySectors();	//Funkcja sprawdza, jak wiele jest sektorow kazdego typu; bool - jezeli nie ma w ogole sektorow dla ktoregos poziomu - error

class cSector
{
	unsigned short width, height;
	eObjType *object;		//Wskaznik do pierwszego obiektu sektora (kazdy obiekt ma swoje id: 0 - pustka, 1 - blok, ...)

public:
	void loadRandomSector(sf::RenderWindow &win, eWorld world_type);	//Losowanie jednego z sektorów danego typu i wczytanie go z pliku
	void loadSector(sf::RenderWindow &win, eWorld world_type, std::string path);		//Wczytywanie sektora z pliku
	bool isSectorFitted(eWorld world_type, cSector &prev_sector, unsigned int level_height);	//Sprawdzanie, czy sektor pasuje do poprzedniego

	unsigned short getWidth();
	unsigned short getHeight();

	eObjType getObject(unsigned short x, unsigned short y);
	void setObject(unsigned short x, unsigned short y, eObjType object); //cSector::

	void clear();	//Wypelnia sektor jedynkami (mozna dopasowac wtedy do kazdego sektora) - stosowany przy tworzeniu pierwszego sektora
};

#endif //!sector_h
