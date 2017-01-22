#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <vector>
#include <string>
#include "graphics.h"
#include "map.h"
#include "sector.h"
#include "global_variables.h"
#include "fonts.h"
#include "menu.h"
#include "dialogs.h"
#include "profile.h"
#include "scoreboard.h"

/*
Errors:
1 - Graphics not found!
2 - Font not found!
3 - Sector not found!
4 - Can't create profile file!
5 - Can't save profile file!
7 - Can't load profile file!
8 - Can't save or create hiscore file!
9 - Can't load hiscore file!
*/

int main()
{
	sf::RenderWindow win(sf::VideoMode(g_width, g_height, 32), "Guardians of the Earth", (g_fullscreen ? sf::Style::Fullscreen : sf::Style::Close));
	win.setFramerateLimit(g_framerate_limit);

	if (!initGraph())
	{
		okDialog(win, "Error 1", "Graphics not found!");
		return 1;
	}
	if (!initFonts())
	{
		okDialog(win, "Error 2", "Font not found!");
		return 2;
	}
	if (!howManySectors())
	{
		okDialog(win, "Error 3", "Sector not found!");
		return 3;
	}
	initControlKeys();

	cProfile profile;
	if (!profile.loadProfile(win, "default"))
		profile.newProfile(win, "default");

	cScoreboard scoreboard[4];	//Tabele wyników oparte o iloœæ graczy
	for (int i = 0; i < 4; i++)
	{
		std::string path = "hiscores";
		path += (char)(i + 1 + 48);
		path += ".dat";
		
		scoreboard[i] = cScoreboard(win, path);
	}
	bool modulators[g_all_modulators] = {};
	
	sf::View p1;
	sf::Event ev;
	
	while (win.isOpen())
	{
		short number_of_players;
		eCharacter character[4];
		if (!mainMenu(win, profile, number_of_players, character, modulators, scoreboard))
			return 0;

		win.clear();
		win.display();

		cMap map(win, WORLD_OVERWORLD, number_of_players, character, modulators);

		p1.setCenter(400, 300);
		p1.setSize(sf::Vector2f(800, 600));
		win.setView(p1);

		//Pêtla gry
		bool game_over = false;
		while (win.isOpen() && !game_over)
		{
			profile.addCash(1);
			//WYDARZENIA
			while (win.pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed)
				{
					if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
						win.close();
				}
			}

			//DZIALANIA W GRZE
			//Poruszanie siê obiektów w poziomie
			if (!map.movements(win, p1, modulators, scoreboard[number_of_players - 1], profile))
				game_over = true;

			//WYSWIETLANIE OBRAZU NA EKRAN
			win.clear(sf::Color(0, 0, 0));

			map.draw(win, p1);
			win.display();
		}

		map.destroy(true);
	}
	shutdownGraph();

	return 0;
}