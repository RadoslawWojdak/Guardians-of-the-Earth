#include "profile.h"

cProfile::cProfile()
{
	this->name = "";
	this->cash = 0;
}

bool cProfile::newProfile(std::string name)
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
		std::cout << "ERROR: Can't create profile file!";
		return false;
	}
	return true;
}

bool cProfile::loadProfile(std::string name)
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
		std::cout << "ERROR: Can't load profile file!";
		return false;
	}
	return true;
}

unsigned int cProfile::getCash()
{
	return this->cash;
}