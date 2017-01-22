#include "profile.h"

cProfile::cProfile()
{
	this->name = "";
	this->cash = 0;

	unlocked_content = new bool*[g_unlocked_types];
	unlocked_content[UNLOCKED_MODULATOR] = new bool[g_unlocked_modulators];

	for (int i = 0; i < g_unlocked_modulators; i++)
		unlocked_content[UNLOCKED_MODULATOR][i] = false;
}

bool cProfile::newProfile(sf::RenderWindow &win, std::string name)
{
	this->name = name;
	
	//Tworzenie folderu i pliku na profil
	std::string path = "profiles/" + name;
	CreateDirectory(path.c_str(), 0);

	std::fstream profile_file;
	path += "/" + name + ".dat";
	profile_file.open(path.c_str(), std::ios::out | std::ios::binary);
	if (profile_file.is_open())
	{
		profile_file.write((char*)&this->cash, sizeof(this->cash));
	}
	else
	{
		okDialog(win, "Error 4", "Can't create profile file!");
		return false;
	}
	return true;
}

bool cProfile::saveProfile(sf::RenderWindow &win)
{
	//Tworzenie folderu i pliku na profil
	std::string path = "profiles/" + this->name + "/" + this->name + ".dat";
	
	std::fstream profile_file;
	profile_file.open(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (profile_file.is_open())
	{
		profile_file.write((char*)&this->cash, sizeof(this->cash));

		profile_file.write((char*)&g_unlocked_types, sizeof(int));
		
		//ODBLOKOWANE MODULATORY ROZGRYWKI
		int unlocked_modulators = 0;	//Iloœæ odblokowanych przez gracza modulatorów rozgrywki
		for (unsigned int i = 0; i < g_unlocked_modulators; i++)
			if (this->unlocked_content[UNLOCKED_MODULATOR][i] == true)
				unlocked_modulators++;
		profile_file.write((char*)&unlocked_modulators, sizeof(int));
		
		for (unsigned int i = 0; i < g_unlocked_modulators; i++)
			if (this->unlocked_content[UNLOCKED_MODULATOR][i] == true)	//Zapisywanie ID modulatorów (od 0)
				profile_file.write((char*)&i, sizeof(i));
	}
	else
	{
		okDialog(win, "Error 5", "Can't save profile file!");
		return false;
	}
	return true;
}

bool cProfile::loadProfile(sf::RenderWindow &win, std::string name)
{
	this->name = name;

	std::string path = "profiles/" + name + "/" + name + ".dat";
	std::fstream profile_file(path.c_str(), std::ios::in | std::ios::binary);
	if (profile_file.is_open())
	{
		profile_file.read((char*)&this->cash, sizeof(unsigned int));

		int unlocked_types;
		profile_file.read((char*)&unlocked_types, sizeof(int));	//Wczytywanie typów mo¿liwych do odblokowania w wersji programu z czasu zapisu profilu

		//WCZYTYWANIE ODBLOKOWANYCH MODULATORÓW ROZGRYWKI
		if (unlocked_types >= 1)
		{
			int unlocked_modulators;
			profile_file.read((char*)&unlocked_modulators, sizeof(unsigned int));
			for (unsigned int i = 0; i < unlocked_modulators; i++)
			{
				unsigned int modulator_id;
				profile_file.read((char*)&modulator_id, sizeof(unsigned int));	//Wczytywanie ID posiadanych przez profil modulatorów
				this->unlocked_content[UNLOCKED_MODULATOR][modulator_id] = true;
			}
		}
	}
	else
	{
		okDialog(win, "Error 6", "Can't Load profile file!");
		return false;
	}
	return true;
}

void cProfile::addCash(unsigned int extra_cash)
{
	unsigned int last_cash = this->cash;

	this->cash += extra_cash;
	if (this->cash < last_cash)		//Je¿eli zmienna by³a ju¿ wymaksowana
		this->cash = 4294967295;	//Maksymalna wartoœæ dla unsigned int
}

bool cProfile::subractractCash(unsigned int subtracted_cash)
{
	if (this->cash >= subtracted_cash)
	{
		cash -= subtracted_cash;
		return true;
	}
	return false;
}


void cProfile::unlockContent(eUnlockedType type, unsigned int ID)
{
	this->unlocked_content[type][ID] = true;
}

bool cProfile::isContentUnlocked(eUnlockedType type, unsigned int ID)
{
	return this->unlocked_content[type][ID];
}


unsigned int cProfile::getCash()
{
	return this->cash;
}

std::string cProfile::getName()
{
	return this->name;
}