#include "shop.h"

cItemShop::cItemShop()
{
	;
}

cItemShop::cItemShop(sf::Vector2f center_pos, unsigned int value, sf::Texture &texture, bool discount)
	:cButton(center_pos, "", texture)
{
	this->discount = discount;
	this->setValue(value * (discount ? 0.75f : 1.0f));
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

bool cItemShop::isDiscount()
{
	return this->discount;
}

void cItemShop::setPosition(sf::Vector2f &position)
{
	this->sf::Sprite::setPosition(position);
	this->text.setPosition(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y + this->getOrigin().y - 4);
}

void cItemShop::setPosition(float x, float y)
{
	this->sf::Sprite::setPosition(x, y);
	this->text.setPosition(this->getPosition().x + this->getOrigin().x - 4, this->getPosition().y + this->getOrigin().y - 4);
}


cShop::cShop(std::vector <cCharacter> &player, bool *modulators)
{
	for (unsigned int i = 0; i < player.size(); i++)
		this->player.push_back(&player[i]);

	short discount = -1;
	if (modulators[2])
		discount = rand() % 5 + 1;

	this->bonus[0] = cItemShop(sf::Vector2f(g_width / 2 - 48, g_height / 2 - 48), rand() % 76 + 250, t_power_up[0], (discount == 1 ? true : false));
	this->bonus[1] = cItemShop(sf::Vector2f(g_width / 2, g_height / 2 - 48), rand() % 86 + 350, t_power_up[1], (discount == 2 ? true : false));
	this->rebirth = cItemShop(sf::Vector2f(g_width / 2 - 48, g_height / 2), rand() % 251 + 1000, t_button_rebirth, (discount == 3 ? true : false));
	this->extra_life = cItemShop(sf::Vector2f(g_width / 2, g_height / 2), rand() % 101 + 500, t_button_extra_life, (discount == 4 ? true : false));
	this->pet_hp = cItemShop(sf::Vector2f(g_width / 2 + 48, g_height / 2), rand() % 51 + 150, t_button_extra_hp, (discount == 5 ? true : false));
	this->exit_button = cButton(sf::Vector2f(g_width / 2, g_height / 2 + 48), "Ready");
}

bool cShop::shopMenu(sf::RenderWindow &win)
{
	win.setView(sf::View(sf::FloatRect(0, 0, g_width, g_height)));

	bool click = true;
	bool end_loop = false;
	bool close_shop[4] = {};
	bool *key_pressed = new bool[this->player.size()];
	short *option = new short[this->player.size()];	//Aktualnie wybrany przycisk przez danego gracza
	for (short i = 0; i < this->player.size(); i++)
	{
		key_pressed[i] = true;
		option[i] = 0;
	}

	sf::Sprite discount_sprite(t_discount_sign);
	discount_sprite.setOrigin(discount_sprite.getTextureRect().width / 2, discount_sprite.getTextureRect().height / 2);

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
		for (int i = 0; i < this->player.size(); i++)
		{
			sControlKeys key = this->player[i]->getControlKeys();

			if (!close_shop[i])
			{
				if (!key_pressed[i])
				{
					if (!key.is_pad)
					{
						if (sf::Keyboard::isKeyPressed(key.up.key))
						{
							key_pressed[i] = true;
							if (option[i] > 2)
								option[i] -= 3;
						}
						if (sf::Keyboard::isKeyPressed(key.down.key))
						{
							key_pressed[i] = true;
							if (option[i] < 6)
							{
								option[i] += 3;
								if (option[i] > 6)
									option[i] = 6;
							}
						}
						if (sf::Keyboard::isKeyPressed(key.left.key))
						{
							key_pressed[i] = true;
							if ((option[i] > 0 && option[i] < 3) || (option[i] > 3 && option[i] < 6))
								option[i]--;
						}
						if (sf::Keyboard::isKeyPressed(key.right.key))
						{
							key_pressed[i] = true;
							if ((option[i] < 1) || (option[i] > 2 && option[i] < 6))
								option[i]++;
						}
					}
					else
					{
						if (sf::Joystick::isButtonPressed(key.pad, key.up.button))
						{
							key_pressed[i] = true;
							if (option[i] > 2)
								option[i] -= 3;
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.down.button))
						{
							key_pressed[i] = true;
							if (option[i] < 6)
								option[i] += 3;
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.left.button))
						{
							key_pressed[i] = true;
							if ((option[i] > 0 && option[i] < 3) || (option[i] > 3 && option[i] < 6))
								option[i]--;
						}
						if (sf::Joystick::isButtonPressed(key.pad, key.right.button))
						{
							key_pressed[i] = true;
							if ((option[i] < 2) || (option[i] > 2 && option[i] < 6))
								option[i]++;
						}
					}
				}

				for (int j = 0; j < 6; j++)
				{
					cItemShop *item = NULL;
					switch (j)
					{
					case 0:
					case 1:
					{
						item = &this->bonus[j];
						break;
					}
					case 3: {item = &this->rebirth; break;}
					case 4: {item = &this->extra_life; break;}
					case 5: {item = &this->pet_hp; break;}
					}

					if (!key_pressed[i])
					{
						if (((!key.is_pad && (sf::Keyboard::isKeyPressed(key.fire.key) || (sf::Keyboard::isKeyPressed(key.jump.key)))) || (key.is_pad && (sf::Joystick::isButtonPressed(key.pad, key.fire.button) || sf::Joystick::isButtonPressed(key.pad, key.jump.button)))) && option[i] == j && this->player[i]->getCash() >= item->getValue())
						{
							key_pressed[i] = true;
							
							if (j == 0 || j == 1)
							{
								this->player[i]->subtractCash(item->getValue());
								this->player[i]->addPower(j + 1);
							}
							else if (item == &this->rebirth)
							{
								for (int k = 0; k < this->player.size(); k++)
								{
									if (!this->player[k]->hasLife())
									{
										this->player[i]->subtractCash(item->getValue());
										this->player[k]->addLife();
										break;
									}
								}
							}
							else if (item == &this->extra_life)
							{
								this->player[i]->subtractCash(item->getValue());
								this->player[i]->addLife();
							}
							else if (item == &this->pet_hp)
							{
								if (this->player[i]->getPet().getHP() < 3)
								{
									this->player[i]->subtractCash(item->getValue());
									this->player[i]->addHP();
								}
							}

							break;
						}
					}
				}
			}

			if (!key_pressed[i])
			{
				if (((!key.is_pad && (sf::Keyboard::isKeyPressed(key.fire.key) || sf::Keyboard::isKeyPressed(key.jump.key))) || (key.is_pad && (sf::Joystick::isButtonPressed(key.pad, key.fire.button) || sf::Joystick::isButtonPressed(key.pad, key.jump.button)))) && option[i] == 6)
				{
					key_pressed[i] = true;
					if (close_shop[i])
						close_shop[i] = false;
					else
						close_shop[i] = true;
				}
			}

			if (key_pressed[i] == true)
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
		for (short i = 0; i < this->player.size(); i++)
		{
			if (!close_shop[i])
			{
				end_loop = false;
				break;
			}
		}

		//WYŒWIETLANIE GRAFIKI
		win.clear();
		
		for (int i = 0; i < this->player.size(); i++)
		{
			sf::Vector2f start;
			start.x = g_width / 2 - (float)(this->player.size() / 2) * (t_stats_window.getSize().x + 32) - 32;
			start.y = g_height / 2 - (t_stats_window.getSize().y + t_button.getSize().y + t_button_extra_hp.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_power_up[0].getSize().y) / 2 + 32;

			this->player[i]->drawStats(win, sf::Vector2f(start.x + 16 + i * (t_stats_window.getSize().y + 48), start.y));

			for (short j = 0; j < 2; j++)
			{
				this->bonus[j].setPosition(start.x + 32 + j * (t_power_up[0].getSize().x + 8) + i * (t_stats_window.getSize().y + 48), start.y + t_stats_window.getSize().y + t_characters_bonus_icon[0][0].getSize().y + 56);
				this->bonus[j].changeGraphics(option[i] == j, sf::Color(255, 192, 0));
				this->bonus[j].draw(win);
				if (this->bonus[j].isDiscount())
				{
					discount_sprite.setPosition(this->bonus[j].getPosition().x - bonus[j].getTextureRect().width / 2 + 2, this->bonus[j].getPosition().y + bonus[j].getTextureRect().height / 2 - 2);
					win.draw(discount_sprite);
				}
			}

			this->rebirth.setPosition(start.x + 32 + i * (t_stats_window.getSize().y + 48), start.y + t_stats_window.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_power_up[0].getSize().y + 64);
			this->rebirth.changeGraphics(option[i] == 3, sf::Color(255, 192, 0));
			this->rebirth.draw(win);
			if (this->rebirth.isDiscount())
			{
				discount_sprite.setPosition(this->rebirth.getPosition().x - rebirth.getTextureRect().width / 2 + 2, this->rebirth.getPosition().y + rebirth.getTextureRect().height / 2 - 2);
				win.draw(discount_sprite);
			}

			this->extra_life.setPosition(start.x + 72 + i * (t_stats_window.getSize().y + 48), start.y + t_stats_window.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_power_up[0].getSize().y + 64);
			this->extra_life.changeGraphics(option[i] == 4, sf::Color(255, 192, 0));
			this->extra_life.draw(win);
			if (this->extra_life.isDiscount())
			{
				discount_sprite.setPosition(this->extra_life.getPosition().x - extra_life.getTextureRect().width / 2 + 2, this->extra_life.getPosition().y + extra_life.getTextureRect().height / 2 - 2);
				win.draw(discount_sprite);
			}

			this->pet_hp.setPosition(start.x + 112 + i * (t_stats_window.getSize().y + 48), start.y + t_stats_window.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_power_up[0].getSize().y + 64);
			this->pet_hp.changeGraphics(option[i] == 5, sf::Color(255, 192, 0));
			this->pet_hp.draw(win);
			if (this->pet_hp.isDiscount())
			{
				discount_sprite.setPosition(this->pet_hp.getPosition().x - pet_hp.getTextureRect().width / 2 + 2, this->pet_hp.getPosition().y + pet_hp.getTextureRect().height / 2 - 2);
				win.draw(discount_sprite);
			}

			this->exit_button.setPosition(start.x + 8 + t_button.getSize().x / 2 + i * (t_stats_window.getSize().y + 48), start.y + t_stats_window.getSize().y + t_characters_bonus_icon[0][0].getSize().y + t_button_extra_hp.getSize().y + t_power_up[0].getSize().y + 72);
			this->exit_button.changeGraphics(option[i] == 6, (close_shop[i] ? sf::Color(64, 255, 64) : sf::Color(255, 192, 0)));
			this->exit_button.draw(win);
		}

		win.display();
	} while (win.isOpen() && !end_loop);
	
	if (win.isOpen())
		return true;
	return false;
}