#include "menu.h"

bool mainMenu(sf::RenderWindow &win, cProfile &profile, short &players, eCharacter character[], bool *modulators_tab, cScoreboard scoreboard[4])
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);
	
	bool click = true;
	bool end_loop = false;
	short option = -1;

	cButton button[6];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 120), "New game");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 72), "Load game");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 24), "High scores");
	button[3] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 24), "Shop");
	button[4] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 72), "Options");
	button[5] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 120), "Quit");

	cButton profile_button;
	profile_button = cButton(sf::Vector2f(g_width - profile_button.getTextureRect().width - 32, 32), "", t_profile_button);
	sf::Text profile_name(profile.getName(), font[1], 30);
	profile_name.setOrigin(0, profile_name.getGlobalBounds().height / 2);
	profile_name.setPosition(g_width - profile_button.getTextureRect().width - 40 - profile_name.getGlobalBounds().width, profile_button.getPosition().y - 8);
	sf::Text profile_cash(uIntToStr(profile.getCash()), font[1], 30);
	profile_cash.setOrigin(0, profile_cash.getGlobalBounds().height / 2);
	profile_cash.setPosition(32, profile_button.getPosition().y - 8);
	profile_cash.setFillColor(sf::Color(255, 215, 0));

	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 6; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				option = i;
				break;
			}
		}
		if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && profile_button.isMouseOver(win))
			option = 6;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		switch (option)
		{
		case 0:	//Nowa gra
		{
			if (menuChooseNumberOfPlayers(win, players, modulators_tab, profile))
			{
				if (menuSelectCharacters(win, players, character, modulators_tab))
					end_loop = true;
			}
			break;
		}
		case 1:	//Wczytaj grê
		{
			break;
		}
		case 2:	//Najwy¿sze wyniki
		{
			menuHighScores(win, scoreboard);
			break;
		}
		case 3: //Sklep
		{
			menuShop(win, profile);
			profile_cash.setString(uIntToStr(profile.getCash()));
			
			break;
		}
		case 4:	//Opcje
		{
			menuOptions(win);
			break;
		}
		case 5: //Wyjœcie
		{
			if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
				return false;
			break;
		}
		case 6:
		{
			menuProfiles(win, profile);
			
			profile_name.setString(profile.getName());
			profile_name.setOrigin(0, profile_name.getGlobalBounds().height / 2);
			profile_name.setPosition(g_width - profile_button.getTextureRect().width - 40 - profile_name.getGlobalBounds().width, profile_button.getPosition().y - 8);

			profile_name.setString(uIntToStr(profile.getCash()));
			profile_name.setOrigin(0, profile_cash.getGlobalBounds().height / 2);
			profile_name.setPosition(32, profile_button.getPosition().y - 8);

			break;
		}
		}
		option = -1;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);
		for (int i = 0; i < 6; i++)
			button[i].draw(win);
		profile_button.draw(win);
		win.draw(profile_name);
		win.draw(profile_cash);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuChooseNumberOfPlayers(sf::RenderWindow &win, short &players, bool *modulators_tab, cProfile &profile)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;
	bool back_pressed = false;

	cButton button[5];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 96), "1");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "2");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2), "3");
	button[3] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "4");
	button[4] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 96), "Back");

	cCheckbox mod_checkbox[g_all_modulators];	//Checkbox'y - dla modulator
	float mod_score_multipler[g_all_modulators];
	mod_checkbox[0] = cCheckbox(sf::Vector2f(0, 0), L"Random multipler amount of NPCs on the map (score multipler: no change)", sf::Color(0, 0, 255), modulators_tab[0]);
	mod_checkbox[1] = cCheckbox(sf::Vector2f(0, 24), L"Extra jump (score multipler: -0.2)", sf::Color(0, 255, 0), modulators_tab[1]);
	mod_checkbox[2] = cCheckbox(sf::Vector2f(0, 48), L"Discount \"-25%\" for a random item in the store (score multipler: -0.3)", sf::Color(0, 255, 0), modulators_tab[2]);
	mod_checkbox[3] = cCheckbox(sf::Vector2f(0, 72), L"HP NPCs x2 (score multipler: +0.5)", sf::Color(255, 0, 0), modulators_tab[3]);
	mod_checkbox[4] = cCheckbox(sf::Vector2f(0, 96), L"Treasures from the crates drop in random direction with random speeds (score multipler: +0.3)", sf::Color(255, 0, 0), modulators_tab[4]);
	mod_checkbox[5] = cCheckbox(sf::Vector2f(0, 120), L"Speed NPCs x2 (score multipler: +0.2)", sf::Color(255, 0, 0), modulators_tab[5]);

	mod_score_multipler[0] = 0.0f;
	mod_score_multipler[1] = -0.2f;
	mod_score_multipler[2] = -0.3f;
	mod_score_multipler[3] = 0.5f;
	mod_score_multipler[4] = 0.3f;
	mod_score_multipler[5] = 0.2f;

	for (int i = 0; i < g_unlocked_modulators; i++)
	{
		cShopItem item(UNLOCKED_MODULATOR, i);
		sModulator modulator = item.getFeatures().modulator;
		
		std::string mulitipler_str;
		std::stringstream ss;
		ss << modulator.score_multipler;
		mulitipler_str = ss.str();
		
		switch (modulator.type)
		{
		case MODULATOR_FACILIATING: {mod_checkbox[6 + i] = cCheckbox(sf::Vector2f(0, 144 + i * 24), item.getDescription() + " (score multipler: " + mulitipler_str + ")", (profile.isContentUnlocked(UNLOCKED_MODULATOR, i) ? sf::Color(0, 255, 0) : sf::Color(128, 128, 128)), modulators_tab[6 + i]); break;}
		case MODULATOR_NEUTRAL: {mod_checkbox[6 + i] = cCheckbox(sf::Vector2f(0, 144 + i * 24), item.getDescription() + " (score multipler: no change)", (profile.isContentUnlocked(UNLOCKED_MODULATOR, i) ? sf::Color(0, 0, 255) : sf::Color(128, 128, 128)), modulators_tab[6 + i]); break;}
		case MODULATOR_OBSTRUCTING: {mod_checkbox[6 + i] = cCheckbox(sf::Vector2f(0, 144 + i * 24), item.getDescription() + " (score multipler: +" + mulitipler_str + ")", (profile.isContentUnlocked(UNLOCKED_MODULATOR, i) ? sf::Color(255, 0, 0) : sf::Color(128, 128, 128)), modulators_tab[6 + i]); break;}
		}

		mod_score_multipler[6 + i] = modulator.score_multipler;
	}

	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		//Dzia³ania na przyciskach
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
					end_loop = true;
					back_pressed = true;
				}
				break;
			}
		}

		//Dzia³ania na checkbox'ach
		for (int i = 0; i < g_all_modulators - g_unlocked_modulators; i++)
		{
			if (mod_checkbox[i].isMouseOver(win))
			{
				if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					mod_checkbox[i].clicked();
					break;
				}
			}
		}
		for (int i = 0; i < g_unlocked_modulators; i++)
		{
			if (mod_checkbox[i + (g_all_modulators - g_unlocked_modulators)].isMouseOver(win) && profile.isContentUnlocked(UNLOCKED_MODULATOR, i))
			{
				if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					mod_checkbox[i + (g_all_modulators - g_unlocked_modulators)].clicked();
					break;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);
		for (int i = 0; i < 5; i++)
			button[i].draw(win);
		for (int i = 0; i < g_all_modulators; i++)
			mod_checkbox[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	g_score_multipler = 1.0f;
	for (int i = 0; i < g_all_modulators; i++)
	{
		modulators_tab[i] = mod_checkbox[i].isChecked();
		g_score_multipler += modulators_tab[i] * mod_score_multipler[i];
	}

	if (win.isOpen() && !back_pressed)
		return true;
	return false;
}

bool menuSelectCharacters(sf::RenderWindow &win, short players, eCharacter character[], bool *modulators_tab)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;
	bool back_pressed = false;

	const int CHARACTERS = g_number_of_characters;
	const int NUMBER_OF_BUTTONS = players * CHARACTERS + 2;

	sf::Sprite *selected_character = new sf::Sprite[players];

	cButton *button = new cButton[NUMBER_OF_BUTTONS];
	for (int i = 0; i < players; i++)
	{
		character[i] = (eCharacter)0;

		selected_character[i].setTexture(t_selected_character);
		selected_character[i].setOrigin(selected_character[i].getTextureRect().width / 2, selected_character[i].getTextureRect().height / 2);
		selected_character[i].setPosition(g_width / 2 - (float)CHARACTERS / 2 * 48 + 22, g_height / 2 + (i - players) * 48);

		for (int j = 0; j < CHARACTERS; j++)
		{
			button[i * CHARACTERS + j] = cButton(sf::Vector2f(0, 0), "", t_character[j]);
			button[i * CHARACTERS + j].setTextureRect(sf::IntRect(66, 0, 22, 32));
			button[i * CHARACTERS + j].setOrigin(button[i * CHARACTERS + j].getTextureRect().width / 2, button[i * CHARACTERS + j].getTextureRect().height / 2);
			button[i * CHARACTERS + j].setPosition(g_width / 2 + (float)(j - (float)CHARACTERS / 2) * 48 + 22, g_height / 2 + (i - players) * 48);
		}
	}
	button[NUMBER_OF_BUTTONS - 2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 8), "Start");
	button[NUMBER_OF_BUTTONS - 1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 40), "Back");

	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		//Dzia³ania na przyciskach
		bool selected = false;
		for (int i = 0; i < players; i++)
		{
			for (int j = 0; j < CHARACTERS; j++)
			{
				if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && button[i * CHARACTERS + j].isMouseOver(win))
				{
					float start_x = g_width / 2 - (float)(players / 2) * (22 * CHARACTERS + 32) - 32;
					selected_character[i].setPosition(g_width / 2 + (float)(j - (float)CHARACTERS / 2) * 48 + 22, g_height / 2 + (i - players) * 48);

					character[i] = (eCharacter)j;
					selected = true;
					break;
				}
			}
			if (selected)
				break;
		}
		//START, BACK BUTTON
		for (int i = 1; i <= 2; i++)
		{
			bool is_mouse_over = button[NUMBER_OF_BUTTONS - i].isMouseOver(win);
			button[NUMBER_OF_BUTTONS - i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				end_loop = true;
				if (i == 1)
					back_pressed = true;
			}
		}
		//!START, BACK BUTTON

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);
		for (int i = 0; i < players; i++)
			win.draw(selected_character[i]);
		for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen() && !back_pressed)
		return true;
	return false;
}

bool menuOptions(sf::RenderWindow &win)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;

	cButton button[3];
	if (g_fullscreen)
		button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "Fullscreen");
	else
		button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "Windowed");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2), "Control Settings");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "Back");


	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 3; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				switch (i)
				{
				case 0:
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

					break;
				}
				case 1:
				{
					menuControlSettings(win);
					break;
				}
				case 2: return false;
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
		win.draw(background);
		for (int i = 0; i < 3; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuControlSettings(sf::RenderWindow &win)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;

	cButton *button = new cButton[4 * 8 + 2];
	for (int i = 0; i < 4; i++)	//Gracze
	{
		if (g_key[i].is_pad == true)
		{
			sf::String pad = "PAD " + (char)(g_key[i].pad + 48);
			unsigned int pad_no = g_key[i].pad;

			button[i * 8] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * 4.5), pad, t_button, sf::Color(0, 128, 0));

			button[i * 8 + 1] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (3.5)), "Up");
			button[i * 8 + 2] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (2.5)), "Down");
			button[i * 8 + 3] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (1.5)), "Left");
			button[i * 8 + 4] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (0.5)), "Right");
			button[i * 8 + 5] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-0.5)), (char)(g_key[i].jump.button + 48));
			button[i * 8 + 6] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-1.5)), (char)(g_key[i].fire.button + 48));
			button[i * 8 + 7] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-2.5)), (char)(g_key[i].special1.button + 48));
		}
		else
		{
			button[i * 8] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (4.5)), "KEYBOARD", t_button, sf::Color(0, 128, 0));

			button[i * 8 + 1] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (3.5)), keyToStr(g_key[i].up.key));
			button[i * 8 + 2] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (2.5)), keyToStr(g_key[i].down.key));
			button[i * 8 + 3] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (1.5)), keyToStr(g_key[i].left.key));
			button[i * 8 + 4] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (0.5)), keyToStr(g_key[i].right.key));
			button[i * 8 + 5] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-0.5)), keyToStr(g_key[i].jump.key));
			button[i * 8 + 6] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-1.5)), keyToStr(g_key[i].fire.key));
			button[i * 8 + 7] = cButton(sf::Vector2f(g_width / 2 - 160 * (1.5 - i), g_height / 2 - 48 * (-2.5)), keyToStr(g_key[i].special1.key));
		}
	}
	button[4 * 8] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48 * (-3.5)), "Back");
	button[4 * 8 + 1] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48 * (-4.5)), "Default");


	sf::Event ev;
	do
	{
		for (int i = 0; i < 4; i++)	//Gracze
		{
			if (g_key[i].is_pad == true)
			{
				std::string button_text("PAD ");
				button_text += (char)(g_key[i].pad + 48);
				button[i * 8].setText(button_text);

				button_text = (g_key[i].jump.button / 10 > 0 ? (char)((g_key[i].jump.button / 10) + 48) : ' ');
				button_text += (char)((g_key[i].jump.button % 10) + 48);
				button[i * 8 + 5].setText(button_text);
				
				button_text = (g_key[i].fire.button / 10 > 0 ? (char)((g_key[i].fire.button / 10) + 48) : ' ');
				button_text += (char)((g_key[i].fire.button % 10) + 48);
				button[i * 8 + 6].setText(button_text);
				
				button_text = (g_key[i].special1.button / 10 > 0 ? (char)((g_key[i].special1.button / 10) + 48) : ' ');
				button_text += (char)((g_key[i].special1.button % 10) + 48);
				button[i * 8 + 7].setText(button_text);
			}
			else
			{
				button[i * 8].setText("KEYBOARD");

				button[i * 8 + 1].setText(keyToStr(g_key[i].up.key));
				button[i * 8 + 2].setText(keyToStr(g_key[i].down.key));
				button[i * 8 + 3].setText(keyToStr(g_key[i].left.key));
				button[i * 8 + 4].setText(keyToStr(g_key[i].right.key));
				button[i * 8 + 5].setText(keyToStr(g_key[i].jump.key));
				button[i * 8 + 6].setText(keyToStr(g_key[i].fire.key));
				button[i * 8 + 7].setText(keyToStr(g_key[i].special1.key));
			}
		}


		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				bool is_mouse_over = button[i * 8 + j].isMouseOver(win);
				button[i * 8 + j].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
				if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
				{
					switch (j)
					{
					case 0:	//Wybieranie urz¹dzenia na którym gracz bêdzie graæ (Klawiatura / Joystick)
					{
						bool selected_device = false;
						do
						{
							win.pollEvent(ev);

							if (ev.type == sf::Event::Closed)
							{
								if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
								{
									win.close();
									break;
								}
							}
							if (ev.type == sf::Event::KeyPressed)
							{
								selected_device = true;
								if (g_key[i].is_pad)
								{
									g_key[i].is_pad = false;

									g_key[i].jump.button = -1;
									g_key[i].fire.button = -1;
									g_key[i].special1.button = -1;
								}
							}

							unsigned int i = 0;
							while (sf::Joystick::isConnected(i))
							{
								unsigned int j = 0;
								while (j < sf::Joystick::getButtonCount(i))
								{
									if (sf::Joystick::isButtonPressed(i, j))
									{
										selected_device = true;
										g_key[i].pad = i;

										if (!g_key[i].is_pad)
										{
											g_key[i].is_pad = true;
											g_key[i].up.button = -1;
											g_key[i].down.button = -1;
											g_key[i].left.button = -1;
											g_key[i].right.button = -1;
											g_key[i].jump.button = -1;
											g_key[i].fire.button = -1;
											g_key[i].special1.button = -1;
										}
									}
									j++;
								}
								i++;
							}
						} while (!selected_device);

						break;
					}		//!Wybieranie urz¹dzenia na którym gracz bêdzie graæ (Klawiatura / Joystick)
					default:	//Wybieranie przycisku za pomoc¹ którego gracz bêdzie wykonywa³ dan¹ czynnoœæ
					{
						bool selected_button = false;
						if (g_key[i].is_pad && (j == 1 || j == 2 || j == 3 || j == 4))	//Na joysticku mo¿na siê poruszaæ jedynie za pomoc¹ osi
							selected_button = true;

						while (!selected_button)
						{
							win.pollEvent(ev);

							if (ev.type == sf::Event::Closed)
							{
								if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
								{
									win.close();
									break;
								}
							}

							if (ev.type == sf::Event::KeyPressed && !g_key[i].is_pad)
							{
								selected_button = true;

								switch (j)
								{
								case 1: {g_key[i].up.key = ev.key.code; break;}
								case 2: {g_key[i].down.key = ev.key.code; break;}
								case 3: {g_key[i].left.key = ev.key.code; break;}
								case 4: {g_key[i].right.key = ev.key.code; break;}
								case 5: {g_key[i].jump.key = ev.key.code; break;}
								case 6: {g_key[i].fire.key = ev.key.code; break;}
								case 7: {g_key[i].special1.key = ev.key.code; break;}
								}
							}
							else if (ev.type == sf::Event::JoystickButtonPressed && ev.joystickButton.joystickId == g_key[i].pad && g_key[i].is_pad)
							{
								selected_button = true;

								switch (j)
								{
								case 5: {g_key[i].jump.button = ev.joystickButton.button; break;}
								case 6: {g_key[i].fire.button = ev.joystickButton.button; break;}
								case 7: {g_key[i].special1.button = ev.joystickButton.button; break;}
								}
							}
						}

						break;
					}	//!Wybieranie przycisku za pomoc¹ którego gracz bêdzie wykonywa³ dan¹ czynnoœæ
					}
				}
			}
		}
		//Przycisk wstecz
		for (int i = 0; i < 2; i++)
		{
			bool is_mouse_over = button[4 * 8 + i].isMouseOver(win);
			button[4 * 8 + i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				switch (i)
				{
				case 0: {end_loop = true; break;}
				case 1: {initControlKeys(); break;}
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);
		for (int i = 0; i < 4 * 8 + 2; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuProfiles(sf::RenderWindow &win, cProfile &profile)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;

	cButton button[3];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "New Profile");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2), "Load Profile");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "Back");


	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < 3; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				switch (i)
				{
				case 0:
				{
					std::string name = textDialog(win, "New Profile", "Enter the player's name.");
					
					if (name != "")
						profile.newProfile(win, name);
					break;
				}
				case 1:
				{
					std::string name = textDialog(win, "Load Profile", "Enter the player's name.");

					if (name != "")
						profile.loadProfile(win, name);
					break;
				}
				case 2:
				{
					return false;
				}
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
		win.draw(background);
		for (int i = 0; i < 3; i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuHighScores(sf::RenderWindow &win, cScoreboard scoreboard[4])
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;

	cButton button[4];
	button[0] = cButton(sf::Vector2f(g_width / 2, 48), "1 Player");
	button[1] = cButton(sf::Vector2f(g_width / 2 - t_button.getSize().x / 2 - t_left_arrow.getSize().x / 2 - 8, 48), "", t_left_arrow);
	button[2] = cButton(sf::Vector2f(g_width / 2 + t_button.getSize().x / 2 + t_right_arrow.getSize().x / 2 + 8, 48), "", t_right_arrow);
	button[3] = cButton(sf::Vector2f(g_width / 2, g_height - 48), "Back");


	short high_score_id = 1;

	sf::Text name[g_scoreboard_size];
	sf::Text score[g_scoreboard_size];

	for (int i = 0; i < g_scoreboard_size; i++)
	{
		name[i].setString(scoreboard[high_score_id - 1].getScoreRegistry(i).name);
		name[i].setCharacterSize(12);
		name[i].setFillColor(sf::Color());
		name[i].setFont(font[1]);
		name[i].setPosition((int)(g_width / 2 - name[i].getGlobalBounds().width - 12), (int)((g_height / 2 - (float)g_scoreboard_size / 2 * 22) + i * 22 + name[i].getGlobalBounds().height / 2));

		std::string score_str = uIntToStr(scoreboard[high_score_id - 1].getScoreRegistry(i).score);
		
		score[i].setString(score_str);
		score[i].setCharacterSize(12);
		score[i].setFillColor(sf::Color());
		score[i].setFont(font[1]);
		score[i].setPosition((int)(g_width / 2 + 12), (int)((g_height / 2 - (float)g_scoreboard_size / 2 * 22) + i * 22 + name[i].getGlobalBounds().height / 2));
	}

	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 1; i < 4; i++)
		{
			bool is_mouse_over = button[i].isMouseOver(win);
			button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			{
				switch (i)
				{
				case 1:
				{
					high_score_id--;
					if (high_score_id < 1)
						high_score_id = 4;

					break;
				}
				case 2:
				{
					high_score_id++;
					if (high_score_id > 4)
						high_score_id = 1;

					break;
				}
				case 3:
				{
					return false;
				}
				}

				//Nazwa tabeli wyników
				sf::String text = (char)(high_score_id + 48);
				text += " Player";
				if (high_score_id != 1)
					text += 's';
				button[0].setText(text);

				//Dostosowanie wyników (aktualna tabela)
				for (int i = 0; i < g_scoreboard_size; i++)
				{
					name[i].setString(scoreboard[high_score_id - 1].getScoreRegistry(i).name);
					name[i].setPosition((int)(g_width / 2 - name[i].getGlobalBounds().width - 12), (int)((g_height / 2 - (float)g_scoreboard_size / 2 * 22) + i * 22 + name[i].getGlobalBounds().height / 2));
					
					std::string score_str = uIntToStr(scoreboard[high_score_id - 1].getScoreRegistry(i).score);
					
					score[i].setString(score_str);
					score[i].setPosition((int)(g_width / 2 + 12), (int)((g_height / 2 - (float)g_scoreboard_size / 2 * 22) + i * 22 + name[i].getGlobalBounds().height / 2));
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);
		for (int i = 0; i < 4; i++)
			button[i].draw(win);
		for (int i = 0; i < g_scoreboard_size; i++)
		{
			win.draw(name[i]);
			win.draw(score[i]);
		}
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}

bool menuShop(sf::RenderWindow &win, cProfile &profile)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));
	sf::Sprite background(t_background[0]);

	bool click = true;
	bool end_loop = false;
	short option = -1;

	int first_displayed = 0;	//Pierwszy przedmiot w sklepie
	const int DISPLAY = 8;		//Ile przedmiotów ma byæ wyœwietlonych
	const int SHOP_MODULATORS = g_unlocked_modulators;
	const int SHOP_ITEMS = SHOP_MODULATORS;	//Wszystkie przedmioty w sklepie

	std::vector <cButton> button;
	std::vector <cShopItem> item;

	int available_modulators = 0;

	for (int i = 0; i < SHOP_MODULATORS; i++)
	{
		if (!profile.isContentUnlocked(UNLOCKED_MODULATOR, i))
		{
			item.push_back(cShopItem(UNLOCKED_MODULATOR, i));
			button.push_back(cButton(sf::Vector2f(g_width / 2, g_height / 2 + (available_modulators - (float)DISPLAY / 2 + 0.5f) * 48), item[available_modulators].getName()));
			available_modulators++;
		}
	}

	button.push_back(cButton(sf::Vector2f(g_width / 2, 48), "Shop"));
	button.push_back(cButton(sf::Vector2f(g_width / 2, g_height / 2 - ((float)DISPLAY / 2 + 0.25f) * 48), "", t_up_arrow));
	button.push_back(cButton(sf::Vector2f(g_width / 2, g_height / 2 + ((float)DISPLAY / 2 + 0.25f) * 48), "", t_down_arrow));
	button.push_back(cButton(sf::Vector2f(g_width / 2, g_height - 48), "Back"));

	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (int i = 0; i < button.size(); i++)
		{
			if (i > item.size() || (i >= first_displayed && i < first_displayed + DISPLAY))
			{
				bool is_mouse_over = button[i].isMouseOver(win);
				button[i].changeGraphics(is_mouse_over, sf::Color(255, 192, 0));
				if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
				{
					option = i;
					break;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		if (option >= 0 && option < available_modulators)	//Modulatory
		{
			if (item[option].viewPurchase(win))
			{
				if (profile.subractractCash(item[option].getPrice()))
				{
					profile.unlockContent(UNLOCKED_MODULATOR, item[option].getFeatures().modulator.id);
					profile.saveProfile(win);

					button.erase(button.begin() + option);
					item.erase(item.begin() + option);
					available_modulators--;
				}
			}
		}


		if (option == item.size() + 1)	//Strza³ka w górê
		{
			first_displayed--;
			if (first_displayed < 0)
				first_displayed = 0;
			else
			{
				for (int i = 0; i < item.size(); i++)
					button[i].setPosition(button[i].getPosition().x, button[i].getPosition().y + 48);
			}
		}
		else if (option == item.size() + 2)	//Strza³ka w dó³
		{
			first_displayed++;
			if (first_displayed + DISPLAY > SHOP_ITEMS)
				first_displayed = first_displayed--;
			else
			{
				for (int i = 0; i < item.size(); i++)
					button[i].setPosition(button[i].getPosition().x, button[i].getPosition().y - 48);
			}
		}
		else if (option == item.size() + 3)	//Wstecz
		{
			return false;
		}
		option = -1;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		win.draw(background);

		//Wyœwietlanie przedmiotów w sklepie
		for (int i = first_displayed; i < first_displayed + DISPLAY; i++)
		{
			if (i >= item.size())
				break;
			else
				button[i].draw(win);
		}

		for (int i = item.size(); i < button.size(); i++)
			button[i].draw(win);
		win.display();
	} while (win.isOpen() && !end_loop);

	if (win.isOpen())
		return true;
	return false;
}


bool menuBetweenLevels(sf::RenderWindow &win, std::vector <cCharacter*>&player)
{
	return true;
}

bool menuSkillTree(sf::RenderWindow &win, std::vector <cCharacter*> &player)
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
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (unsigned short i = 0; i < player.size(); i++)
		{
			sControlKeys key = g_key[player[i]->getPlayerNo() - 1];
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
						key_pressed[i] = true;
						if (player[i]->getSkillPoints() > 0)
						{
							key_pressed[i] = true;
							switch (option[i])
							{
							case 1:
							case 3:
							{
								player[i]->addSkill(option[i]);
								break;
							}
							case 2:
							{
								if (player[i]->getLevel() >= 5)
									player[i]->addSkill(option[i]);
								break;
							}
							case 4:
							{
								if (player[i]->getLevel() >= 10)
									player[i]->addSkill(option[i]);
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
						key_pressed[i] = true;
						if (player[i]->getSkillPoints() > 0)
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
								player[i]->addSkill(option[i]);
								break;
							}
							case 3:
							{
								if (player[i]->getLevel() >= 5)
									player[i]->addSkill(option[i]);
								break;
							}
							case 4:
							{
								if (player[i]->getLevel() >= 10)
									player[i]->addSkill(option[i]);
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

			player[i]->drawSkillTree(win, sf::Vector2f(start.x + 16 + i * (t_stats_window.getSize().y + 48), start.y), option[i], close_menu[i]);
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


bool menuPause(sf::RenderWindow &win)
{
	//Przechwycenie obrazu z rozgrywki (bêdzie robi³o za t³o wokó³ Menu Pauzy)
	sf::Texture texture_background;
	texture_background.create(g_width, g_height);
	texture_background.update(win);
	sf::Sprite background(texture_background);

	//Ustawienie widoku na punkt od (0; 0) (dziêki temu mo¿na dzia³aæ na pozycjach od punktu (0; 0) przy rysowaniu menu
	sf::View start_view = win.getView();	//Poprzedni widok musi byæ zapisany, ¿eby gra nie przenosi³a gracza w okolice punktu (0; 0)
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	//Tworzenie t³a Menu Pauzy
	sf::Sprite window(t_pause_menu_window);
	window.setOrigin(sf::Vector2f(window.getTextureRect().width / 2, window.getTextureRect().height / 2));
	window.setPosition(sf::Vector2f(g_width / 2, g_height / 2));

	//Tworzenie przycisków
	cButton button[3];
	button[0] = cButton(sf::Vector2f(g_width / 2, g_height / 2 - 48), "Back to game");
	button[1] = cButton(sf::Vector2f(g_width / 2, g_height / 2), "Options");
	button[2] = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "Exit game");

	//Pêtla menu
	bool click = true;
	bool key_pressed = true;
	bool end_loop = false;
	sf::Event ev;
	do
	{
		//WYDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//DZIA£ANIA NA MENU
		for (unsigned short i = 0; i < 3; i++)
		{
			button[i].changeGraphics(button[i].isMouseOver(win), sf::Color(255, 215, 0));
			if (button[i].isMouseOver(win) && !click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				switch (i)
				{
				case 0: {end_loop = true; break;}
				case 1:
				{
					if (menuOptions(win))
						return false;
					break;
				}
				case 2: {return false; break;}
				}
				break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//Za pomoc¹ przycisku Escape, gracz mo¿e wyjœæ z menu i wróciæ do rozgrywki
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			key_pressed = false;
		else if (!key_pressed)
			end_loop = true;

		//WYŒWIETLANIE GRAFIKI
		win.clear();

		win.draw(background);
		win.draw(window);
		for (unsigned short i = 0; i < 3; i++)
			button[i].draw(win);

		win.display();
	} while (win.isOpen() && !end_loop);

	win.setView(start_view);

	if (win.isOpen())
		return true;
	return false;
}