#ifndef profile_h
#define profile_h

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>

class cProfile
{
	std::string name;
	unsigned int cash;
	
public:
	cProfile();

	bool newProfile(std::string name);
	bool loadProfile(std::string name);

	unsigned int getCash();
};

#endif //!profile_h