#include "sector.h"
#include <cstdlib>
#include <iostream>

unsigned int how_many_sectors[7];

bool howManySectors()
{
	std::string path;
	std::string type;
	std::fstream file;

	for (int i = 0; i < 7; i++)
		how_many_sectors[i] = 0;
	
	for (int i = 0; i < 7; i++)
	{
		switch (i)
		{
			case WORLD_OVERWORLD:		type = "overworld";		break;
			case WORLD_UNDERGROUND:		type = "underground";	break;
			/*case underwater:	type = "underwater";	break;
			case desert:		type = "desert";		break;
			case ice_land:		type = "ice_land";		break;
			case hot_land:		type = "hot_land";		break;
			case sky:			type = "sky";			break;*/
		}

		for (int j = 0; ; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();

			path = "sectors\\" + type + "\\sector-" + nr + ".sec";
			file.open((char*)path.c_str(), std::ios::in);
			if (!file.is_open())		//Jezeli udalo sie otworzyc plik, to znaczy, ze sektor istnieje
			{
				if (j == 0)
					return false;

				how_many_sectors[i] = j;
				break;
			}
			file.close();
		}
	}

	return true;
}

void cSector::loadRandomSector(eWorld type, std::string &id)
{
	//Dok³adniejsze losowanie liczb (w oparciu o mniejsze liczby od sekund)
	clock_t t1;
	t1 = clock();
	
	static unsigned long long rnd = t1;
	srand(rnd);
	rnd = rand() % 1844674407370955161 + t1;

	std::string path;

	path = "sectors\\";
	switch (type)
	{
		case WORLD_OVERWORLD:		path += "overworld";	break;
		case WORLD_UNDERGROUND:		path += "underground";	break;
		/*case underwater:	path += "underwater";	break;
		case desert:		path += "desert";		break;
		case ice_land:		path += "ice_land";		break;
		case hot_land:		path += "hot_land";		break;
		case sky:			path += "sky";			break;
	*/}
	path += "\\sector-";

	std::string nr;
	std::stringstream ss;
	ss << rand()%how_many_sectors[type]+1;
	nr = ss.str();

	path += nr + ".sec";

	this->loadSector(path);

	id = nr;
}

void cSector::loadSector(std::string path)
{
	//eObjType* object;
	bool state[9];		//czy dany stan istnienia ma byc wlaczony? (dane po kropce w sektorze)
	for (int i = 0; i < 9; i++)
		state[i] = rand() % 2;	//true/false

	std::fstream file;
	file.open((char*)path.c_str(), std::ios::in);
	if (file.is_open())
	{
		std::string text;
		std::stringstream ss;

		//Wczytywanie wymiarow sektora
		file >> text;
		ss.str(text);
		ss >> this->width;
		ss.clear();

		file >> text;
		ss.str(text);
		ss >> this->height;
		ss.clear();
		
		this->object = new eObjType[this->width*this->height];

		//Wczytywanie obiektow sektora
		for (int i = 0; i < this->height; i++)
			for (int j = 0; j < this->width; j++)
			{
				int id;
				file >> text;
				ss.str(text);
				ss >> id;
				ss.clear();

				//Sprawdzanie stanu istnienia oraz losowanie, czy ten stan istnienia ma zaistniec
				short dot_pos = text.find('.');
				short dot_pos2;
				short dot_pos3;
				
				if (dot_pos >= 0 && dot_pos <= 256)	//Jezeli znaleziono kropke w tekscie
				{
					std::string text2;		//Tekst po kropce
					text2.insert(0, text, dot_pos + 1, text.length() - dot_pos - 1);
					
					int state_nr;
					ss.str(text2);
					ss >> state_nr;
					ss.clear();

					dot_pos2 = text.find('.', dot_pos + 1);

					//Jezeli po drugiej kropce jest 0, a stan istnienia jest wlaczony, to obiekt sie nie pojawia
					if (dot_pos2 >= 0 && dot_pos2 <= 256)
					{
						//Sprawdzanie czy po kropce jest 0
						text2 = "";		//Tekst po kropce
						text2.insert(0, text, dot_pos2 + 1, text.length() - dot_pos2 - 1);

						int is_exists;
						ss.str(text2);
						ss >> is_exists;
						ss.clear();

						//Jezeli stan istnienia zostal wylosowany, ale obiekt jest domyslnie wylaczony, to nadal pozostaje wylaczony (pozostaje pusta przestrzen)
						if ((state[state_nr] && is_exists == 0) || (!state[state_nr] && is_exists != 0))
						{
							id = 0;

							dot_pos3 = text.find('.', dot_pos2 + 1);// text.find('.', dot_pos2 + 1);

							//Jezeli isnieje trzecia kropka, to obiekt jest zamieniany na ten z ID po trzeciej kropce. Jezeli nie istnieje, to zostaje put¹ przestrzeni¹
							if (dot_pos3 >= 0 && dot_pos3 <= 256)
							{
								//Sprawdzanie numeru po kropce
								text2 = "";		//Tekst po kropce
								text2.insert(0, text, dot_pos3 + 1, text.length() - dot_pos3 - 1);

								int object;
								ss.str(text2);
								ss >> object;
								ss.clear();

								id = object;
							}
						}
					}
					//Jezeli nawet nie ma drugiej kropki
					else if (!state[state_nr])	//Jezeli ten stan istnienia nie zostal wylosowany, to zamienia objekt na pusta przestrzen
						id = 0;
				}

				*(this->object + i*width + j) = (eObjType)id;
			}
		/*
		for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width; j++)
				std::cout << *(this->object + i*width + j);
			std::cout << "\n";
		}*/

		//SPRAWDZANIE NOWYCH WYMIARÓW SEKTORA
		/*for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width; j++)
			{
				std::cout << this->getObject(j, i) << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";*/

		//Sprawdzanie jak wysoki jest sektor po zmianach stanu
		bool found = false;
		unsigned short decrease_height = 0;	//O ile ma sie zmniejszyc sektor
		for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width + 1; j++)
			{
				if (this->getObject(j, i) != none)		//Jezeli na tej wysokoœci znajduje siê jakiœ obiekt, to poziom ma tê wysokoœæ
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
			decrease_height++;
		}
		//Zmniejszeni wysokoœci sektora
		//this->height -= decrease;

		//Sprawdzanie jak szeroki jest sektor po zmianach stanu
		found = false;
		unsigned short decrease_width = 0;
		for (int i = this->width - 1; i >= 0; i--)
		{
			for (int j = 0; j < this->height; j++)
			{
				if (this->getObject(i, j) != none)		//Jezeli na tej wysokoœci znajduje siê jakiœ obiekt, to poziom ma tê szerokoœæ
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
			decrease_width++;
		}

		//std::cout << this->width << ":" << this->height << "\n";
		//std::cout << decrease_width << ":" << decrease_height << "\n";

		//USUWANIE WSZYSTKICH PÓL SPOZA SZEROKOŒCI I WYSOKOŒCI SEKTORA
		eObjType *temp = new eObjType[this->width * this->height];
		for (int i = 0; i < this->height; i++)
			for (int j = 0; j < this->width; j++)
			{
				*(temp + i * this->width + j) = *(this->object + i * this->width + j);
			}
		delete this->object;

		//Zmniejszenie sektora
		unsigned short last_width = this->width, last_height = this->height;
		this->height -= decrease_height;
		this->width -= decrease_width;

		this->object = new eObjType[this->width * this->height];
		
		for (int i = decrease_height; i < last_height; i++)
			for (int j = 0; j < this->width; j++)
			{
				this->setObject(j, i - decrease_height, *(temp + i * last_width + j));
			}
		delete temp;

		/*for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width; j++)
			{
				std::cout << this->getObject(j, i) << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		system("PAUSE");*/
	}
	else
	{
		exit(3);
	}
}

bool cSector::isSectorFitted(cSector &prev_sector, unsigned int level_height)
{
	//LISTA OBIEKTÓW (OD GÓRY DO DO£U POZIOMU) PRAWEGO KOÑCA LEWEGO SEKTORA I LEWEGO KOÑCA PRAWEGO SEKTORA!!!
	//iloœæ obiektów - zale¿na od wysokoœci poziomu
	unsigned short objects_count = level_height / 32 + (level_height % 32 == 0 ? 0 : 1) - 1;

	eObjType **object = new eObjType*[2];	//Pierwszy wymiar tablicy - pion (0 - koniec poprzedniego sektora; 1 - pocz¹tek aktualnego sektora); drugi wymiar tablicy - obiekty od góry do do³u
	for (int i = 0; i < 2; i++)
		object[i] = new eObjType[objects_count];

	//Obiekty poprzedniego sektora
	for (int i = objects_count - 1; i >= 0; i--)
	{
		if (prev_sector.getHeight() - (objects_count - i) >= 0)
			object[0][i] = prev_sector.getObject(prev_sector.getWidth() - 1, prev_sector.getHeight() - (objects_count - i));
		//Je¿eli sektor ju¿ siê skoñczy³, to wype³nia resztê tablicy zerami (pust przestrzeñ)
		else
			object[0][i] = none;
	}
	//Obiekty aktualnego sektora
	for (int i = objects_count - 1; i >= 0; i--)
	{
		if (this->getHeight() - (objects_count - i) >= 0)
			object[1][i] = this->getObject(0, this->getHeight() - (objects_count - i));
		//Je¿eli sektor ju¿ siê skoñczy³, to wype³nia resztê tablicy zerami (pust przestrzeñ)
		else
			object[1][i] = none;
	}

	//PÊTLA SPRAWDZANIA DOPASOWANIA SEKTORA
	for (int i = objects_count - 1; i >= objects_count - prev_sector.getHeight(); i--)
	{
		switch (object[0][i])		//id obiektu znajdujacego sie na ostatniej lini sektora
		{
		case ground:
		case block:
			if ((object[0][i - 1] == none || object[0][i - 1] == treasure || object[0][i - 1] == bonus_block) && (object[0][i - 2] == none || object[0][i - 2] == treasure || object[0][i - 2] == bonus_block))	//Je¿eli ponad gruntem s¹ conajmniej 2 puste pola
			{
				if ((((object[1][i] == none || object[1][i] == treasure || object[1][i] == bonus_block) || (object[1][i - 2] == none || object[1][i - 2] == treasure || object[1][i - 2] == bonus_block)) && (object[1][i - 1] == none || object[1][i - 1] == treasure || object[1][i - 1] == bonus_block)) && (object[1][i] == ground || object[1][i] == block || object[1][i + 1] == ground || object[1][i + 1] == block))	//Je¿eli na dwóch polach na równi, o stopieñ wy¿ej lub ni¿ej mo¿na przejœæ, a na równi lub stopieñ ni¿ej jest grunt lub blok, to gracz bêdzie móg³ przejœc
					return true;
			}
			break;
		}
	}

	return false;
}

unsigned short cSector::getWidth()
{
	return this->width;
}

unsigned short cSector::getHeight()
{
	return this->height;
}

eObjType cSector::getObject(unsigned short x, unsigned short y)
{
	if ((x >= this->width) || (x < 0) || (y >= this->height) || (y < 0))	//W razie, jezeli zostanie sprawdzone nieistniejace pole w danym sektorze (potrzebne przy generowaniu terenu)
		return none;
	
	return *(this->object + x + y*this->width);
}

void cSector::setObject(unsigned short x, unsigned short y, eObjType object)
{
	*(this->object + x + y*this->width) = object;
}

void cSector::clear()
{
	this->width = 1;
	this->height = 10;
	this->object = new eObjType[10];	//wysoki na 10 (wtedy mozna dobierac nowe sektory)
	for (int i = 0; i < 10; i++)
		*(this->object + i) = ground;
}