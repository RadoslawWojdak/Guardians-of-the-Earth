#include "dialogs.h"

void okDialog(sf::RenderWindow &win, sf::String head, sf::String description)
{
	//Przechwycenie obrazu z rozgrywki (bêdzie robi³o za t³o wokó³ Menu Pauzy)
	sf::Texture texture_background;
	texture_background.create(g_width, g_height);
	texture_background.update(win);
	sf::Sprite background(texture_background);

	//Ustawienie widoku na punkt od (0; 0) (dziêki temu mo¿na dzia³aæ na pozycjach od punktu (0; 0) przy rysowaniu menu
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	//Pozosta³e grafiki, napisy i przyciski
	sf::Sprite dial_win(t_dialog_window);
	dial_win.setPosition(g_width / 2 - dial_win.getTextureRect().width / 2, g_height / 2 - dial_win.getTextureRect().height / 2);

	sf::Text text_head(head, font[1], 16);
	text_head.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 4);
	sf::Text text_description(description, font[1], 16);
	text_description.setFillColor(sf::Color(0, 0, 0));
	text_description.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 32);

	cButton close_button(sf::Vector2f((int)(dial_win.getPosition().x + dial_win.getTextureRect().width - t_close_button.getSize().x / 2 - 4), (int)(dial_win.getPosition().y + t_close_button.getSize().y / 2 + 4)), "", t_close_button);
	cButton ok_button(sf::Vector2f(dial_win.getPosition().x + (int)dial_win.getTextureRect().width / 2, dial_win.getPosition().y + dial_win.getTextureRect().height - t_button.getSize().y / 2 - 16), "OK");
	
	//Zmienne do pêtli
	sf::Event ev;
	bool end_loop = false;
	bool click = true;
	
	//Pêtla g³ówna okna dialogowego
	do
	{
		//ZDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//G£ÓWNA CZÊŒÆ PÊTLI
		ok_button.changeGraphics(ok_button.isMouseOver(win), sf::Color(255, 215, 0));
		
		if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			click = true;
			if (close_button.isMouseOver(win) || ok_button.isMouseOver(win))
				end_loop = true;
		}
		else if (click && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
			click = false;

		//WYŒWIETLANIE OKNA NA EKRANIE
		win.clear();

		win.draw(background);
		win.draw(dial_win);

		close_button.draw(win);
		ok_button.draw(win);
		
		win.draw(text_head);
		win.draw(text_description);

		win.display();
	} while (!end_loop && win.isOpen());
}

bool yesNoDialog(sf::RenderWindow &win, sf::String head, sf::String description)
{
	//Przechwycenie obrazu z rozgrywki (bêdzie robi³o za t³o wokó³ Menu Pauzy)
	sf::Texture texture_background;
	texture_background.create(g_width, g_height);
	texture_background.update(win);
	sf::Sprite background(texture_background);

	//Ustawienie widoku na punkt od (0; 0) (dziêki temu mo¿na dzia³aæ na pozycjach od punktu (0; 0) przy rysowaniu menu
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	//Pozosta³e grafiki, napisy i przyciski
	sf::Sprite dial_win(t_dialog_window);
	dial_win.setPosition(g_width / 2 - dial_win.getTextureRect().width / 2, g_height / 2 - dial_win.getTextureRect().height / 2);

	sf::Text text_head(head, font[1], 16);
	text_head.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 4);
	sf::Text text_description(description, font[1], 16);
	text_description.setFillColor(sf::Color(0, 0, 0));
	text_description.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 32);

	cButton close_button(sf::Vector2f((int)(dial_win.getPosition().x + dial_win.getTextureRect().width - t_close_button.getSize().x / 2 - 4), (int)(dial_win.getPosition().y + t_close_button.getSize().y / 2 + 4)), "", t_close_button);
	cButton yes_button(sf::Vector2f(dial_win.getPosition().x + t_button.getSize().x / 2 + 16, dial_win.getPosition().y + dial_win.getTextureRect().height - t_button.getSize().y / 2 - 16), "YES");
	cButton no_button(sf::Vector2f(dial_win.getPosition().x + dial_win.getTextureRect().width - t_button.getSize().x / 2 - 16, dial_win.getPosition().y + dial_win.getTextureRect().height - t_button.getSize().y / 2 - 16), "NO");

	//Zmienne do pêtli
	sf::Event ev;
	bool end_loop = false;
	bool click = true;
	bool selected; //0 - NO, 1 - YES
	//Pêtla g³ówna okna dialogowego
	do
	{
		//ZDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();
		}

		//G£ÓWNA CZÊŒÆ PÊTLI
		no_button.changeGraphics(no_button.isMouseOver(win), sf::Color(255, 215, 0));
		yes_button.changeGraphics(yes_button.isMouseOver(win), sf::Color(255, 215, 0));

		if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			click = true;
			if (close_button.isMouseOver(win) || no_button.isMouseOver(win))
			{
				selected = false;
				end_loop = true;
			}
			else if (yes_button.isMouseOver(win))
			{
				selected = true;
				end_loop = true;
			}
		}
		else if (click && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
			click = false;

		//WYŒWIETLANIE OKNA NA EKRANIE
		win.clear();

		win.draw(background);
		win.draw(dial_win);
		
		close_button.draw(win);
		yes_button.draw(win);
		no_button.draw(win);

		win.draw(text_head);
		win.draw(text_description);

		win.display();
	} while (!end_loop && win.isOpen());

	if (!win.isOpen() || selected == 0)
		return false;
	else
		return true;
}

std::string textDialog(sf::RenderWindow &win, sf::String head, sf::String description)
{
	//Przechwycenie obrazu z rozgrywki (bêdzie robi³o za t³o wokó³ Menu Pauzy)
	sf::Texture texture_background;
	texture_background.create(g_width, g_height);
	texture_background.update(win);
	sf::Sprite background(texture_background);

	//Ustawienie widoku na punkt od (0; 0) (dziêki temu mo¿na dzia³aæ na pozycjach od punktu (0; 0) przy rysowaniu menu
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	//Pozosta³e grafiki, napisy i przyciski
	sf::Sprite dial_win(t_dialog_window);
	dial_win.setPosition(g_width / 2 - dial_win.getTextureRect().width / 2, g_height / 2 - dial_win.getTextureRect().height / 2);

	sf::Text text_head(head, font[1], 16);
	text_head.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 4);
	sf::Text text_description(description, font[1], 16);
	text_description.setFillColor(sf::Color(0, 0, 0));
	text_description.setPosition(dial_win.getPosition().x + 4, dial_win.getPosition().y + 32);
	sf::Text user_text("", font[1], 16);
	user_text.setFillColor(sf::Color(0, 0, 0));
	user_text.setOrigin(user_text.getGlobalBounds().width / 2, user_text.getOrigin().y);
	user_text.setPosition(dial_win.getPosition().x + dial_win.getTextureRect().width / 2, dial_win.getPosition().y + 96);

	cButton close_button(sf::Vector2f((int)(dial_win.getPosition().x + dial_win.getTextureRect().width - t_close_button.getSize().x / 2 - 4), (int)(dial_win.getPosition().y + t_close_button.getSize().y / 2 + 4)), "", t_close_button);
	cButton ok_button(sf::Vector2f(dial_win.getPosition().x + t_button.getSize().x / 2 + 16, dial_win.getPosition().y + dial_win.getTextureRect().height - t_button.getSize().y / 2 - 16), "OK");
	cButton cancel_button(sf::Vector2f(dial_win.getPosition().x + dial_win.getTextureRect().width - t_button.getSize().x / 2 - 16, dial_win.getPosition().y + dial_win.getTextureRect().height - t_button.getSize().y / 2 - 16), "CANCEL");

	//Zmienne do pêtli
	sf::Event ev;
	bool end_loop = false;
	bool click = true;
	bool selected; //0 - CANCEL, 1 - OK
				   //Pêtla g³ówna okna dialogowego
	do
	{
		//ZDARZENIA
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				if (yesNoDialog(win, L"Exit", L"Do you want to leave the game?"))
					win.close();

			if (ev.type == sf::Event::KeyPressed)
			{
				sf::Keyboard::Key key = ev.key.code;

				//Koniec pisania
				if (key == sf::Keyboard::Return)
				{
					selected = 1;
					end_loop = true;
				}
				//Dopisywanie tekstu
				if (user_text.getString().toAnsiString().length() < 16)
				{
					if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
						user_text.setString(user_text.getString() + keyToStr(key));
					else if ((key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) || (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9))
					{
						sf::String letter = keyToStr(key);
						letter.erase(0, keyToStr(key).toAnsiString().length() - 1);
						user_text.setString(user_text.getString() + letter);
					}
				}
				//Usuwanie tekstu
				if (user_text.getString().toAnsiString().length() > 0)
				{
					if (key == sf::Keyboard::BackSpace)
					{
						sf::String erased = user_text.getString();
						erased.erase(user_text.getString().toAnsiString().length() - 1);
						user_text.setString(erased);
					}
				}
			}
		}

		//G£ÓWNA CZÊŒÆ PÊTLI
		user_text.setOrigin(user_text.getGlobalBounds().width / 2, user_text.getOrigin().y);
		user_text.setPosition(dial_win.getPosition().x + dial_win.getTextureRect().width / 2, dial_win.getPosition().y + 96);
		
		cancel_button.changeGraphics(cancel_button.isMouseOver(win), sf::Color(255, 215, 0));
		ok_button.changeGraphics(ok_button.isMouseOver(win), sf::Color(255, 215, 0));

		if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			click = true;
			if (close_button.isMouseOver(win) || cancel_button.isMouseOver(win))
			{
				selected = false;
				end_loop = true;
			}
			else if (ok_button.isMouseOver(win))
			{
				selected = true;
				end_loop = true;
			}
		}
		else if (click && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
			click = false;

		//WYŒWIETLANIE OKNA NA EKRANIE
		win.clear();

		win.draw(background);
		win.draw(dial_win);

		close_button.draw(win);
		ok_button.draw(win);
		cancel_button.draw(win);

		win.draw(text_head);
		win.draw(text_description);
		win.draw(user_text);

		win.display();
	} while (!end_loop && win.isOpen());

	if (!win.isOpen() || selected == 0)
		return "";
	return user_text.getString().toAnsiString();
}