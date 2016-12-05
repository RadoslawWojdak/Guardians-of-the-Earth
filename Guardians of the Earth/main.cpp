#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <vector>
#include "graphics.h"
#include "map.h"
#include "sector.h"
#include "global_variables.h"
//#include "ground.h"

/*
0 - overworld
1 - underground
2 - underwater
3 - desert
4 - ice land
5 - hot land
6 - sky
*/

//STWORZYÆ TYLKO Z SAMEGO GRUNTU CIA£O STATYCZNE BIBLIOTEKI BOX2D - KLASA TERRAIN?
//NADAÆ KA¯DEMU BLOKOWI I BONUSOWEMU BLOKOWI CIA£O

#include <iostream>
#include <cstdlib>

int main()
{
	sf::RenderWindow win(sf::VideoMode(g_width, g_height, 32), "Guardians of the Earth", sf::Style::Close);
	win.setFramerateLimit(60);

	if (!init_graph())
	{
		std::cout << "Blad wczytywania grafik!\n";
		system("PAUSE");
		return 1;
	}
	if (!howManySectors())
	{
		std::cout << "Blad wczytywania sektorow!\n";
		system("PAUSE");
		return 2;
	}

	sf::Event ev;

	cMap map(WORLD_OVERWORLD);

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
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
		win.setView(p1);
		//std::cout << p1.getViewport().width << " " << p1.getViewport().width << "\n";
		//Poruszanie siê obiektów w poziomie
		map.movements(p1);

		//WYSWIETLANIE OBRAZU NA EKRAN
		win.clear(sf::Color(0, 0, 0));

		map.draw(win, p1);
		win.display();
	}

	map.destroy();

	return 0;
}