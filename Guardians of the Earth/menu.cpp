#include "menu.h"

bool mainMenu(sf::RenderWindow &win, short &players)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

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
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
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
			if (menuChooseNumberOfPlayers(win, players))
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
			menuOptions(win);
			option = -1;
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

bool menuChooseNumberOfPlayers(sf::RenderWindow &win, short &players)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

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
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
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

bool menuOptions(sf::RenderWindow &win)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	bool click = true;
	bool end_loop = false;

	class cButton button[2];
	if (g_fullscreen)
		button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 24), "Fullscreen");
	else
		button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 24), "Windowed");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 24), "Back");


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
		for (int i = 0; i < 2; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				if (i == 0)
				{
					if (g_fullscreen)
					{
						g_fullscreen = false;
						button[0].setText("Windowed");
					}
					else
					{
						g_fullscreen = true;
						button[0].setText("Fullscreen");
					}

					win.close();
					win.create(sf::VideoMode(g_width, g_height, 32), "Guardians of the Earth", (g_fullscreen ? sf::Style::Fullscreen : sf::Style::Close));
					win.setFramerateLimit(g_framerate_limit);
				}
				else if (i == 1)
					return false;
				break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		for (int i = 0; i < 2; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}


bool menuBetweenLevels(sf::RenderWindow &win, std::vector <cCharacter> &player)
{
	return true;
}

bool menuSkillTree(sf::RenderWindow &win, std::vector <cCharacter> &player)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	unsigned short *option = new unsigned short[player.size()];
	bool *key_pressed = new bool[player.size()];
	bool *close_menu = new bool[player.size()];
	for (short i = 0; i < player.size(); i++)
	{
		option[i] = 0;
		key_pressed[i] = true;
		close_menu[i] = false;
	}

	bool end_loop = false;
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
		for (unsigned short i = 0; i < player.size(); i++)
		{
			sControlKeys key = player[i].getControlKeys();
			if (!key_pressed[i])
			{
				if (!key.is_pad)
				{
					if (!close_menu[i])
					{
						if (sf::Keyboard::isKeyPressed(key.up.key))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 0: {option[i] = 4; break;}
							case 2:
							case 4: {option[i]--; break;}
							}
						}
						if (sf::Keyboard::isKeyPressed(key.down.key))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 3: {option[i]++; break;}
							case 2:
							case 4: {option[i] = 0; break;}
							}
						}
						if (sf::Keyboard::isKeyPressed(key.left.key))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 3:
							case 4: {option[i] -= 2; break;}
							}
						}
						if (sf::Keyboard::isKeyPressed(key.right.key))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 2: {option[i] += 2; break;}
							}
						}
					} //!if (!close_menu[i])
					
					if (sf::Keyboard::isKeyPressed(key.jump.key) || sf::Keyboard::isKeyPressed(key.fire.key))
					{
						if (player[i].getSkillPoints() > 0)
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 3:
							{
								player[i].addSkill(option[i]);
								break;
							}
							case 2:
							{
								if (player[i].getLevel() >= 5)
									player[i].addSkill(option[i]);
								break;
							}
							case 4:
							{
								if (player[i].getLevel() >= 10)
									player[i].addSkill(option[i]);
								break;
							}
							}
						}

						if (option[i] == 0)
						{
							if (!close_menu[i])
								close_menu[i] = true;
							else
								close_menu[i] = false;
						}
					}
				}
				else
				{
					if (!close_menu[i])
					{
						if (sf::Joystick::isButtonPressed(key.pad, key.up.button))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 0: {option[i] = 4; break;}
							case 2:
							case 4: {option[i]--; break;}
							}
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.down.button))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 3: {option[i]++; break;}
							case 2:
							case 4: {option[i] = 0; break;}
							}
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.left.button))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 3:
							case 4: {option[i] -= 2; break;}
							}
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.right.button))
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 2: {option[i] += 2; break;}
							}
						}
					} //!if (close_menu[i])

					if (sf::Joystick::isButtonPressed(key.pad, key.jump.button) || sf::Joystick::isButtonPressed(key.pad, key.fire.button))
					{
						if (player[i].getSkillPoints() > 0)
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 0:
							{
								if (!close_menu[i])
									close_menu[i] = true;
								else
									close_menu[i] = false;
								break;
							}
							case 1:
							case 2:
							{
								player[i].addSkill(option[i]);
								break;
							}
							case 3:
							{
								if (player[i].getLevel() >= 5)
									player[i].addSkill(option[i]);
								break;
							}
							case 4:
							{
								if (player[i].getLevel() >= 10)
									player[i].addSkill(option[i]);
								break;
							}
							}
						}
					}
				}
			}

			if (key_pressed[i])
			{
				if (!key.is_pad)
				{
					if (!(sf::Keyboard::isKeyPressed(key.up.key)) && !(sf::Keyboard::isKeyPressed(key.down.key)) && !(sf::Keyboard::isKeyPressed(key.left.key)) && !(sf::Keyboard::isKeyPressed(key.right.key)) && !(sf::Keyboard::isKeyPressed(key.jump.key)) && !(sf::Keyboard::isKeyPressed(key.fire.key)))
						key_pressed[i] = false;
				}
				else
				{
					if (!(sf::Joystick::isButtonPressed(key.is_pad, key.up.button)) && !(sf::Joystick::isButtonPressed(key.is_pad, key.down.button)) && !(sf::Joystick::isButtonPressed(key.is_pad, key.left.button)) && !(sf::Joystick::isButtonPressed(key.is_pad, key.right.button)) && !(sf::Joystick::isButtonPressed(key.is_pad, key.jump.button)) && !(sf::Joystick::isButtonPressed(key.is_pad, key.fire.button)))
						key_pressed[i] = false;
				}
			}
		}

		end_loop = true;
		for (unsigned short i = 0; i < player.size(); i++)
			if (!close_menu[i])
				end_loop = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		
		for (int i = 0; i < player.size(); i++)
		{
			sf::Vector2f start;
			start.x = g_width / 2 - (float)(player.size() / 2) * (t_stats_window.getSize().x + 32) - 32;
			start.y = g_height / 2 - (t_stats_window.getSize().y + t_button.getSize().y + t_button_extra_hp.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_power_up[0].getSize().y) / 2 + 32;

			player[i].drawSkillTree(win, start, option[i], close_menu[i]);
		}

		win.display();
	} while (win.isOpen() && !end_loop);

	delete[] option;
	delete[] key_pressed;
	delete[] close_menu;

	if (win.isOpen())
		return true;
	return false;
}