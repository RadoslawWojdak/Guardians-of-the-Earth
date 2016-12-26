#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <vector>
#include "graphics.h"
#include "map.h"
#include "sector.h"
#include "global_variables.h"
#include "fonts.h"

#include <iostream>
#include <cstdlib>

int main()
{
	//Tymczasowy sposób wyboru iloœci graczy
	short player_number;
	std::cout << "Wybierz ilosc graczy: ";
	std::cin >> player_number;

	//Tymczasowy sposób wyboru typu poziomu
	int option;
	do
	{
		system("CLS");
		std::cout << "Wybierz typ poziomu:\n"
			<< "1. Overworld\n"
			<< "2. Underground\n"
			<< "3. Underwater\n"
			<< "4. Ice land\n"
			<< "5. Desert\n"
			<< "Twoj wybor: ";
		std::cin >> option;
	} while (option < 1 || option > 5);
	option--;
	//!Tymczasowy sposób wyboru typu poziomu

	sf::RenderWindow win(sf::VideoMode(g_width, g_height, 32), "Guardians of the Earth", sf::Style::Close);
	win.setFramerateLimit(60);

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

	sf::Event ev;

	cMap map((eWorld)option, player_number);

	sf::View p1;
	p1.setCenter(400, 300);
	p1.setSize(sf::Vector2f(800, 600));
	win.setView(p1);

	//Pêtla gry
	while (win.isOpen())
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				win.close();
		}

		//DZIALANIA W GRZE
		//Poruszanie siê obiektów w poziomie
		map.movements(p1);

		//WYSWIETLANIE OBRAZU NA EKRAN
		win.clear(sf::Color(0, 0, 0));

		map.draw(win, p1);
		win.display();
	}

	map.destroy(true);
	shutdownGraph();

	return 0;
}