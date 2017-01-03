#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <vector>
#include "graphics.h"
#include "map.h"
#include "sector.h"
#include "global_variables.h"
#include "fonts.h"
#include "menu.h"

#include <iostream>
#include <cstdlib>

int main()
{
	sf::RenderWindow win(sf::VideoMode(g_width, g_height, 32), "Guardians of the Earth", (g_fullscreen ? sf::Style::Fullscreen : sf::Style::Close));
	win.setFramerateLimit(g_framerate_limit);

	if (!initGraph())
	{
		std::cout << "Blad wczytywania grafik!\n";
		system("PAUSE");
		return 1;
	}
	if (!initFonts())
	{
		std::cout << "Blad wczytywania czcionek!\n";
		system("PAUSE");
		return 4;
	}
	if (!howManySectors())
	{
		std::cout << "Blad wczytywania sektorow!\n";
		system("PAUSE");
		return 2;
	}

	sf::View p1;
	sf::Event ev;
	
	while (win.isOpen())
	{
		short number_of_players;
		if (!mainMenu(win, number_of_players))
			return 0;
		win.clear();
		win.display();

		cMap map(WORLD_ICE_LAND, number_of_players);

		p1.setCenter(400, 300);
		p1.setSize(sf::Vector2f(800, 600));
		win.setView(p1);

		//Pêtla gry
		bool game_over = false;
		while (win.isOpen() && !game_over)
		{
			//WYDARZENIA
			while (win.pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed)
					win.close();
			}

			//DZIALANIA W GRZE
			//Poruszanie siê obiektów w poziomie
			if (!map.movements(win, p1))
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