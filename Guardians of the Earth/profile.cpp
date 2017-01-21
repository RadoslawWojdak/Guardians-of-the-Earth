#include "profile.h"

cProfile::cProfile()
{
	this->name = "";
	this->cash = 0;
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
	if (this->cash < last_cash)		//Je�eli zmienna by�a ju� wymaksowana
		this->cash = 4294967295;	//Maksymalna warto�� dla unsigned int
}

unsigned int cProfile::getCash()
{
	return this->cash;
}

std::string cProfile::getName()
{
	return this->name;
}