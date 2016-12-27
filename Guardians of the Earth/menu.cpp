#include "menu.h"

bool mainMenu(sf::RenderWindow &win, short &players)
{
	bool click = true;
	bool end_loop = false;
	short option = -1;

	class cButton button[4];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 72), "New game");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 24), "Load game");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 24), "Options");
	button[3] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 72), "Quit");

	
	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 4; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over);
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				option = i;
				break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		switch (option)
		{
		case 0:	//Nowa gra
		{
			if (menuChooseNumerOfPlayers(win, players))
				end_loop = true;
			else
				option = -1;
			break;
		}
		case 1:	//Wczytaj grê
		{
			break;
		}
		case 2:	//Opcje
		{
			break;
		}
		case 3: //Wyjœcie
		{
			return false;
		}
		}

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		for (int i = 0; i < 4; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuChooseNumerOfPlayers(sf::RenderWindow &win, short &players)
{
	bool click = true;
	bool end_loop = false;

	class cButton button[5];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 96), "1");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "2");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2), "3");
	button[3] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "4");
	button[4] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 96), "Back");


	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 5; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over);
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				if (i < 4)
				{
					players = i + 1;
					end_loop = true;
				}
				else
				{
					players = -1;
					return false;
				}
				break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		for (int i = 0; i < 5; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}