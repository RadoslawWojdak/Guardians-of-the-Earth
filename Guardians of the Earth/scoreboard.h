#ifndef files_h
#define files_h

#include <fstream>
#include <string>
#include "dialogs.h"
#include "global_variables.h"

struct sScoreRegistry
{
	std::string name;
	unsigned int score;
};

class cScoreboard
{
	sScoreRegistry registry[g_scoreboard_size];

public:
	cScoreboard();
	cScoreboard(sf::RenderWindow &win, std::string path);

	bool saveScoreboard(std::string path);
	bool loadScoreboard(std::string path);

	sScoreRegistry getScoreRegistry(unsigned short registry_id);
	void appendScore(std::string name, unsigned int score);
};

#endif //!files_h