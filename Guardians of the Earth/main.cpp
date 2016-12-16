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

	cMap map((eWorld)option, 1);
	
	sf::View p1;
	p1.setCenter(400, 300);
	p1.setSize(sf::Vector2f(800, 600));
	win.setView(p1);

	while (win.isOpen())
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				win.close();
		}

		//DZIALANIA W GRZE
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			p1.setCenter(sf::Vector2f(p1.getCenter().x + 7, p1.getCenter().y));
			if (p1.getCenter().x > map.getWidth() - 400)
				p1.setCenter(map.getWidth() - 400, p1.getCenter().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			p1.setCenter(sf::Vector2f(p1.getCenter().x - 7, p1.getCenter().y));
			if (p1.getCenter().x < 400)
				p1.setCenter(sf::Vector2f(400, p1.getCenter().y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			p1.setCenter(sf::Vector2f(p1.getCenter().x, p1.getCenter().y - 7));
			if (p1.getCenter().y < 300)
				p1.setCenter(sf::Vector2f(p1.getCenter().x, 300));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			p1.setCenter(sf::Vector2f(p1.getCenter().x, p1.getCenter().y + 7));
			if (p1.getCenter().y > map.getHeight() - 300)
				p1.setCenter(sf::Vector2f(p1.getCenter().x, map.getHeight() - 300));
		}
		win.setView(p1);*/

		//Poruszanie siê obiektów w poziomie
		map.movements(p1);

		//WYSWIETLANIE OBRAZU NA EKRAN
		win.clear(sf::Color(0, 0, 0));

		map.draw(win, p1);
		win.display();
	}

	map.destroy();
	shutdownGraph();

	return 0;
}