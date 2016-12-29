#include "shop.h"

cItemShop::cItemShop()
{
	;
}

cItemShop::cItemShop(sf::Vector2f center_pos, unsigned int value, sf::Texture &texture)
	:cButton(center_pos, "", texture)
{
	this->setValue(value);
}

void cItemShop::setValue(unsigned int value)
{
	this->value = value;
	
	std::stringstream ss;
	ss << value;
	
	this->text.setFont(font[1]);
	this->text.setCharacterSize(8);
	this->text.setString(ss.str());
	this->text.setFillColor(sf::Color(64, 64, 64));
	this->text.setOrigin(this->text.getGlobalBounds().width / 2, this->text.getGlobalBounds().height / 2);
	this->text.setPosition(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y + this->getOrigin().y - 4);
}

unsigned int cItemShop::getValue()
{
	return this->value;
}


cShop::cShop(std::vector <cCharacter> &player)
{
	for (unsigned int i = 0; i < player.size(); i++)
		this->player.push_back(&player[i]);

	this->rebirth = cItemShop(sf::Vector2f(g_width / 2 - 48, g_height / 2), rand() % 251 + 1000, t_button_rebirth);
	this->extra_life = cItemShop(sf::Vector2f(g_width / 2, g_height / 2), rand() % 101 + 500, t_button_extra_life);
	this->pet_hp = cItemShop(sf::Vector2f(g_width / 2 + 48, g_height / 2), rand() % 51 + 150, t_button_extra_hp);
	this->exit_button = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "Exit");
}

bool cShop::shopMenu(sf::RenderWindow &win)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	bool click = true;
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
		for (int i = 0; i < 3; i++)
		{
			cItemShop *item = NULL;
			switch (i)
			{
			case 0: {item = &this->rebirth; break;}
			case 1: {item = &this->extra_life; break;}
			case 2: {item = &this->pet_hp; break;}
			}

			bool is_mouse_over = item->isMouseOver(win);
			item->changeGraphics(is_mouse_over);
			if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over && this->player[0]->getCash() >= item->getValue())
			{
				
				if (item == &this->rebirth)
				{
					for (int i = 0; i < this->player.size(); i++)
					{
						if (!this->player[i]->hasLife())
						{
							this->player[0]->subtractCash(item->getValue());
							this->player[i]->addLife();
							break;
						}
					}
				}
				else if (item == &this->extra_life)
				{
					this->player[0]->subtractCash(item->getValue());
					this->player[0]->addLife();
				}
				else if (item == &this->pet_hp)
				{
					if (this->player[0]->getPet().getHP() < 3)
					{
						this->player[0]->subtractCash(item->getValue());
						this->player[0]->addHP();
					}
				}

				break;
			}
		}
		bool is_mouse_over = this->exit_button.isMouseOver(win);
		this->exit_button.changeGraphics(is_mouse_over);
		if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_mouse_over)
			end_loop = true;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			click = true;
		else
			click = false;

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		
		for (int i = 0; i < this->player.size(); i++)
			this->player[i]->drawStats(win, sf::Vector2f(16 + i * (t_stats_window.getSize().y + 32), 16));

		this->rebirth.draw(win);
		this->extra_life.draw(win);
		this->pet_hp.draw(win);
		this->exit_button.draw(win);
		
		win.display();
	} while (win.isOpen() && !end_loop);
	
	if (win.isOpen())
		return true;
	return false;
}