#include "scoreboard.h"

cScoreboard::cScoreboard()
{
	;
}

cScoreboard::cScoreboard(sf::RenderWindow &win, std::string path)
{
	if (!this->loadScoreboard(path))
	{
		std::fstream new_file;

		for (int i = 0; i < g_scoreboard_size; i++)
		{
			this->registry[i].name = "NONE";
			this->registry[i].score = 0;
		}

		if (!saveScoreboard(path))
		{
			okDialog(win, "Error 7", "Can't save or create hiscore file!");
			exit(6);
		}
	}
}

bool cScoreboard::saveScoreboard(std::string path)
{
	std::fstream file;

	file.open((char*)path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (file.is_open())
	{
		for (int i = 0; i < g_scoreboard_size; i++)
		{
			int name_length = this->registry[i].name.length();
			file.write((char*)&name_length, sizeof(this->registry[i].name.length()));

			for (int j = 0; j < name_length; j++)
				file.write((char*)&this->registry[i].name[j], sizeof(this->registry[i].name[j]));

			file.write((char*)&this->registry[i].score, sizeof(this->registry[i].score));
		}
	}
	else
		return false;
	file.close();
	return true;
}

bool cScoreboard::loadScoreboard(std::string path)
{
	for (int i = 0; i < g_scoreboard_size; i++)
	{
		this->registry[i].name = "";
		this->registry[i].score = 0;
	}
	
	std::fstream file;

	file.open((char*)path.c_str(), std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		for (int i = 0; i < g_scoreboard_size; i++)
		{
			int name_length;
			file.read((char*)&name_length, sizeof(int));

			for (int j = 0; j < name_length; j++)
			{
				char character;
				file.read((char*)&character, sizeof(char));
				this->registry[i].name += character;
			}

			file.read((char*)&this->registry[i].score, sizeof(int));
		}
	}
	else
		return false;
	file.close();
	return true;
}

sScoreRegistry cScoreboard::getScoreRegistry(unsigned short registry_id)
{
	if (registry_id >= 0 && registry_id < g_scoreboard_size)
		return this->registry[registry_id];
}

void cScoreboard::appendScore(std::string name, unsigned int score)
{
	short pos = -1;

	for (int i = 0; i < g_scoreboard_size; i++)
	{
		if (score > this->registry[i].score)
		{
			pos = i;
			break;
		}
	}

	if (pos != -1)
	{
		for (int i = g_scoreboard_size - 2; i > pos - 1; i--)
		{
			this->registry[i + 1].name = this->registry[i].name;
			this->registry[i + 1].score = this->registry[i].score;
		}
		this->registry[pos].name = name;
		this->registry[pos].score = score;
	}
}

bool cScoreboard::isSufficientlyHighScore(unsigned int score)
{
	if (score > this->registry[g_scoreboard_size - 1].score)
		return true;
	return false;
}