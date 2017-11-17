#include "profile.h"

cProfile::cProfile()
{
	this->name = "";

	unlocked_content = new bool*[g_unlocked_types];
	unlocked_content[UNLOCKED_MODULATOR] = new bool[g_unlocked_modulators];
	unlocked_content[UNLOCKED_NPC] = new bool[g_unlocked_npcs];

	for (int i = 0; i < g_unlocked_modulators; i++)
		unlocked_content[UNLOCKED_MODULATOR][i] = false;
	for (int i = 0; i < g_unlocked_npcs; i++)
		unlocked_content[UNLOCKED_NPC][i] = false;

	this->save_name.clear();
}

bool cProfile::newProfile(sf::RenderWindow &win, std::string name)
{
	this->name = name;

	//Tworzenie folderu i pliku na profil
	std::string path = "profiles/" + name;

	#ifdef WINDOWS
        CreateDirectory(path.c_str(), 0);
    #endif
    #ifdef linux
        std::string linux_path="mkdir -p " + path;
        system(linux_path.c_str());
    #endif

	std::fstream profile_file;
	path += "/" + name + ".dat";
	profile_file.open(path.c_str(), std::ios::out | std::ios::binary);
	if (profile_file.is_open())
	{
		profile_file.write((char*)&this->cash, sizeof(this->cash));

		int zero = 0;
		profile_file.write((char*)&zero, sizeof(int));	//Zero przedmiotów do odblokowania
		profile_file.write((char*)&zero, sizeof(int));	//Zero slotów zapisu
	}
	else
	{
		okDialog(win, "Error 4", "Can't create profile file!");
		return false;
	}
	profile_file.close();
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

		//ODBLOKOWANE NPC-y
		int unlocked_npcs = 0;	//Iloœæ odblokowanych przez gracza NPC-ów
		for (unsigned int i = 0; i < g_unlocked_npcs; i++)
			if (this->unlocked_content[UNLOCKED_NPC][i] == true)
				unlocked_npcs++;
		profile_file.write((char*)&unlocked_npcs, sizeof(int));

		for (unsigned int i = 0; i < g_unlocked_modulators; i++)
			if (this->unlocked_content[UNLOCKED_NPC][i] == true)	//Zapisywanie ID modulatorów (od 0)
				profile_file.write((char*)&i, sizeof(i));

		//NAZWY SLOTÓW ZAPISU
		int number_of_save_slots = this->save_name.size();
		profile_file.write((char*)&number_of_save_slots, sizeof(int));
		for (unsigned int i = 0; i < number_of_save_slots; i++)
		{
			int name_length = this->save_name[i].size();
			profile_file.write((char*)&name_length, sizeof(int));
			for (int j = 0; j < name_length; j++)
				profile_file.write((char*)&this->save_name[i][j], sizeof(char));
		}
	}
	else
	{
		okDialog(win, "Error 5", "Can't save profile file!");
		return false;
	}
	profile_file.close();
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

		//WCZYTYWANIE ODBLOKOWANYCH NPC-ów
		if (unlocked_types >= 2)
		{
			int unlocked_npcs;
			profile_file.read((char*)&unlocked_npcs, sizeof(unsigned int));
			for (unsigned int i = 0; i < unlocked_npcs; i++)
			{
				unsigned int npc_id;
				profile_file.read((char*)&npc_id, sizeof(unsigned int));	//Wczytywanie ID posiadanych przez profil NPC-ów
				this->unlocked_content[UNLOCKED_NPC][npc_id] = true;
			}
		}

		//NAZWY SLOTÓW ZAPISU
		int number_of_save_slots;
		profile_file.read((char*)&number_of_save_slots, sizeof(int));
		for (unsigned int i = 0; i < number_of_save_slots; i++)
		{
			int name_length;
			profile_file.read((char*)&name_length, sizeof(int));

			std::string name = "";
			for (int j = 0; j < name_length; j++)
			{
				char character;
				profile_file.read((char*)&character, sizeof(char));
				name += character;
			}
			this->save_name.push_back(name);
		}
	}
	else
	{
		okDialog(win, "Error 6", "Can't Load profile file!");
		return false;
	}
	profile_file.close();
	return true;
}

void cProfile::addSaveSlot(sf::RenderWindow &win, std::string slot_name)
{
	bool slots_name_exists = false;
	for (int i = 0; i < this->save_name.size(); i++)
		if (this->save_name[i] == slot_name)
			slots_name_exists = true;

	if (!slots_name_exists)
	{
		this->save_name.push_back(slot_name);
		this->saveProfile(win);
	}
}

void cProfile::deleteSaveSlot(sf::RenderWindow &win, std::string slot_name)
{
	for (int i = 0; i < this->save_name.size(); i++)
		if (this->save_name[i] == slot_name)
		{
			this->save_name.erase(this->save_name.begin() + i);

			std::string path = "profiles/" + this->name + "/saves/" + slot_name + ".dat";
			remove((char*)path.c_str());
			break;
		}
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

std::string cProfile::getSaveSlotName(unsigned int id)
{
	return this->save_name[id];
}

unsigned int cProfile::getNumberOfSaveSlots()
{
	return this->save_name.size();
}
