#ifndef sector_h
#define sector_h

#include <vector>
#include <fstream>
#include <sstream>	//Konwersja liczby na tekst - wczytywanie numeru sektora
#include <ctime>	//Losowanie sektora
#include <string>	//Dzialanie na danych sektora (po kropce)
#include "enums.h"

extern unsigned int how_many_sectors[7];	//Jak wiele jest sektorow danego typu; bool - jezeli nie ma w ogole sektorow dla ktoregos poziomu - error

/*enum eSecType	//typ sektora - overworld/underground/...
{
	overworld = 0,
	underground = 1,
	underwater = 2,
	desert = 3,
	ice_land = 4,
	hot_land = 5,
	sky = 6
};*/

bool howManySectors();	//Funkcja sprawdza, jak wiele jest sektorow kazdego typu

class cSector
{
	unsigned short width, height;
	eObjType *object;		//Wskaznik do pierwszego obiektu sektora (kazdy obiekt ma swoje id - 0 - pustka, 1 - block, ...)

public:
	void loadRandomSector(eWorld type, std::string &id);	//Losowanie jednego z sektor�w danego typu i wczytanie go z pliku
	void loadSector(std::string path);		//Wczytywanie sektora z pliku
	bool isSectorFitted(cSector &prev_sector, unsigned int level_height);	//Sprawdzanie, czy sektor pasuje do poprzedniego

	unsigned short getWidth();
	unsigned short getHeight();

	eObjType getObject(unsigned short x, unsigned short y);
	void cSector::setObject(unsigned short x, unsigned short y, eObjType object);

	void clear();	//Wypelnia sektor jedynkami (mozna dopasowac wtedy do kazdego sektora) - stosowany przy tworzeniu pierwszego sektora
};

#endif //!sector_h