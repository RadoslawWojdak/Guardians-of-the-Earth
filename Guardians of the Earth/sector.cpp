#include "sector.h"

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
			case WORLD_OVERWORLD: {type = "overworld";		break;}
			case WORLD_UNDERGROUND: {type = "underground";	break;}
			case WORLD_UNDERWATER: {type = "underwater";	break;}
			case WORLD_ICE_LAND: {type = "ice_land";		break;}
			case WORLD_DESERT: {type = "desert";			break;}
		}

		for (int j = 0; ; j++)
		{
			std::string nr;
			std::stringstream ss;
			ss << j + 1;
			nr = ss.str();

			path = "sectors/" + type + "/sector-" + nr + ".sec";
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

void cSector::loadRandomSector(sf::RenderWindow &win, eWorld world_type)
{
	//Dok³adniejsze losowanie liczb (w oparciu o mniejsze liczby od sekund)
	clock_t t1;
	t1 = clock();

	static unsigned long long rnd = t1;
	srand(rnd);
	rnd = rand() % 1844674407370955161 + t1;

	std::string path;

	path = "sectors/";
	switch (world_type)
	{
		case WORLD_OVERWORLD: {path += "overworld";		break;}
		case WORLD_UNDERGROUND: {path += "underground";	break;}
		case WORLD_UNDERWATER: {path += "underwater";	break;}
		case WORLD_ICE_LAND: {path += "ice_land";		break;}
		case WORLD_DESERT: {path += "desert";			break;}
	}
	path += "/sector-";

	std::string nr;
	std::stringstream ss;
	ss << rand()%how_many_sectors[world_type]+1;
	nr = ss.str();

	path += nr + ".sec";

	this->loadSector(win, world_type, path);
}

void cSector::loadSector(sf::RenderWindow &win, eWorld world_type, std::string path)
{
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
					else if (!state[state_nr])	//Jezeli ten stan istnienia nie zostal wylosowany, to zamienia objekt na pusta przestrzen (wodê w przypadku œwiata wodnego)
					{
						if (world_type == WORLD_UNDERWATER)
							id = 4;
						else
							id = 0;
					}
				}

				*(this->object + i*width + j) = (eObjType)id;
			}

		//Sprawdzanie jak wysoki jest sektor po zmianach stanu
		bool found = false;
		unsigned short decrease_height = 0;	//O ile ma sie zmniejszyc sektor
		for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width + 1; j++)
			{
				if (this->getObject(j, i) != OBJECT_NONE)		//Jezeli na tej wysokoœci znajduje siê jakiœ obiekt, to poziom ma tê wysokoœæ
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
			decrease_height++;
		}

		//Sprawdzanie jak szeroki jest sektor po zmianach stanu
		found = false;
		unsigned short decrease_width = 0;
		for (int i = this->width - 1; i >= 0; i--)
		{
			for (int j = 0; j < this->height; j++)
			{
				if (this->getObject(i, j) != OBJECT_NONE)		//Jezeli na tej wysokoœci znajduje siê jakiœ obiekt, to poziom ma tê szerokoœæ
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
			decrease_width++;
		}

		//USUWANIE WSZYSTKICH PÓL SPOZA SZEROKOŒCI I WYSOKOŒCI SEKTORA
		eObjType *temp = new eObjType[this->width * this->height];
		for (int i = 0; i < this->height; i++)
			for (int j = 0; j < this->width; j++)
			{
				*(temp + i * this->width + j) = *(this->object + i * this->width + j);
			}
		delete[] this->object;

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
		delete[] temp;
	}
	else
	{
		std::string world_str;

		switch (world_type)
		{
		case WORLD_OVERWORLD: {world_str = "Overworld"; break;}
		case WORLD_UNDERGROUND: {world_str = "Underground"; break;}
		case WORLD_UNDERWATER: {world_str = "Underwater"; break;}
		case WORLD_ICE_LAND: {world_str = "Ice land"; break;}
		case WORLD_DESERT: {world_str = "Desert";	break;}
		}

		sf::String error = L"Not found sector for level ";
		error += world_str;
		okDialog(win, L"Error 3", error);
		exit(3);
	}
}

bool cSector::isSectorFitted(eWorld world_type, cSector &prev_sector, unsigned int level_height)
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
		{
			object[0][i] = prev_sector.getObject(prev_sector.getWidth() - 1, prev_sector.getHeight() - (objects_count - i));

			//Zamienia objekty za którymi kryje siê dany typ terenu na ten typ terenu
			if (object[0][i] == OBJECT_BONUS_BLOCK || object[0][i] == OBJECT_TREASURE || object[0][i] == OBJECT_TRAMPOLINE || object[0][i] == OBJECT_POWER_UP)
			{
				if (world_type == WORLD_UNDERWATER)
					object[0][i] = OBJECT_FLUID;
				else
					object[0][i] = OBJECT_NONE;
			}
			//W œwiecie "Ice Land" woda jest traktowana jak blok
			else if (object[0][i] == OBJECT_FLUID && world_type == WORLD_ICE_LAND)
				object[0][i] = OBJECT_GROUND;
		}
		//Je¿eli sektor ju¿ siê skoñczy³, to wype³nia resztê tablicy zerami (pust przestrzeñ), lub gruntem (w przypadku œwiata podziemnego)
		else
		{
			if (world_type == WORLD_UNDERGROUND)
				object[0][i] = OBJECT_GROUND;
			else if (world_type == WORLD_UNDERWATER)
				object[0][i] = OBJECT_FLUID;
			else
				object[0][i] = OBJECT_NONE;
		}
	}
	//Obiekty aktualnego sektora
	for (int i = objects_count - 1; i >= 0; i--)
	{
		if (this->getHeight() - (objects_count - i) >= 0)
		{
			object[1][i] = this->getObject(0, this->getHeight() - (objects_count - i));

			//Zamienia objekty za którymi kryje siê dany typ terenu na ten typ terenu
			if (object[1][i] == OBJECT_BONUS_BLOCK || object[1][i] == OBJECT_TREASURE || object[1][i] == OBJECT_TRAMPOLINE || object[1][i] == OBJECT_POWER_UP)
			{
				if (world_type == WORLD_UNDERWATER)
					object[1][i] = OBJECT_FLUID;
				else
					object[1][i] = OBJECT_NONE;
			}
			//W œwiecie "Ice Land" woda jest traktowana jak blok
			else if (object[1][i] == OBJECT_FLUID && world_type == WORLD_ICE_LAND)
				object[1][i] = OBJECT_GROUND;
		}
		//Je¿eli sektor ju¿ siê skoñczy³, to wype³nia resztê tablicy zerami (pust przestrzeñ) lub gruntem (w przypadku œwiata podziemnego)
		else if (world_type == WORLD_UNDERGROUND)
			object[1][i] = OBJECT_GROUND;
		else if (world_type == WORLD_UNDERWATER)
			object[1][i] = OBJECT_FLUID;
		else
			object[1][i] = OBJECT_NONE;
	}

	//PÊTLA SPRAWDZANIA DOPASOWANIA SEKTORA
	if (world_type != WORLD_UNDERWATER)	//W przypadku gdy œwiat nie jest œwiatem podwodnym, to generator musi sprawdzaæ, czy da siê przejœæ chodz¹c
	{
		for (int i = objects_count - 1; i >= objects_count - prev_sector.getHeight(); i--)
		{
			switch (object[0][i])		//id obiektu znajdujacego sie na ostatniej lini sektora
			{
			case OBJECT_GROUND:
			case OBJECT_BLOCK:
			{
				if ((object[0][i - 1] == OBJECT_NONE || object[0][i - 1] == OBJECT_LADDER) && (object[0][i - 2] == OBJECT_NONE || object[0][i - 2] == OBJECT_LADDER))	//Je¿eli ponad gruntem s¹ conajmniej 2 puste pola
				{
					if ((((object[1][i] == OBJECT_NONE || object[1][i] == OBJECT_LADDER) || (object[1][i - 2] == OBJECT_NONE || object[1][i - 2] == OBJECT_LADDER)) && (object[1][i - 1] == OBJECT_NONE || object[1][i - 1] == OBJECT_LADDER)) && (object[1][i] == OBJECT_GROUND || object[1][i] == OBJECT_BLOCK || object[1][i + 1] == OBJECT_GROUND || object[1][i + 1] == OBJECT_BLOCK))	//Je¿eli na dwóch polach na równi, o stopieñ wy¿ej lub ni¿ej mo¿na przejœæ, a na równi lub stopieñ ni¿ej jest grunt lub blok, to gracz bêdzie móg³ przejœc
						return true;
				}
				break;
			}
			}
		}
	}
	else	//Je¿eli œwiat jest œwiatem podwodnym, to wystarczy, ¿e bêdzie siê da³o przep³yn¹æ
	{
		for (int i = objects_count - 1; i >= 0; i--)	//Pêtla przebiega po wszystkich elementach, gdy¿ zawsze mo¿na przep³yn¹æ gór¹
		{
			switch (object[0][i])
			{
			case OBJECT_FLUID:
			{
				if ((object[0][i - 1] == OBJECT_FLUID || object[0][i - 1] == OBJECT_LADDER) && (object[0][i - 2] == OBJECT_FLUID || object[0][i - 2] == OBJECT_LADDER))	//Je¿eli powy¿ej te¿ znajduje siê woda
				{
					if ((object[1][i] == OBJECT_FLUID || object[1][i] == OBJECT_LADDER) && ((object[1][i - 1] == OBJECT_FLUID || object[1][i - 1] == OBJECT_LADDER) || (object[1][i + 1] == OBJECT_FLUID || object[1][i + 1] == OBJECT_LADDER)))
						return true;
				}
				break;
			}
			}
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
		return OBJECT_NONE;

	return *(this->object + x + y*this->width);
}

void cSector::setObject(unsigned short x, unsigned short y, eObjType object)
{
	*(this->object + x + y*this->width) = object;
}

void cSector::clear()
{
	this->width = 1;
	this->height = 3;
	this->object = new eObjType[3];	//wysoki na 3

	for (int i = 0; i < 2; i++)
		*(this->object + i) = OBJECT_NONE;
	*(this->object + 2) = OBJECT_GROUND;
}
