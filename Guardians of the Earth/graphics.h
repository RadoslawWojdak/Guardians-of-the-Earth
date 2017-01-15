#ifndef graphics_h
#define graphics_h

#include "SFML\Graphics.hpp"
#include <sstream>	//Konwersja liczby na tekst

extern sf::Texture t_block[2];
extern sf::Texture t_bonus_block[2];
extern sf::Texture t_gold_bonus_block[2];
extern sf::Texture t_ground[5][47];	//Pierwszy wymiar tablicy - rodzaj podloza overground; drugi - ID podloza
extern sf::Texture t_treasure[8];
extern sf::Texture t_power_up[2];
extern sf::Texture t_background[5];
extern sf::Texture *t_background_obj[5];
extern sf::Texture t_npc[5];
extern sf::Texture t_object[8];
extern sf::Texture t_character[2];
extern sf::Texture t_characters_bonus[2][2];
extern sf::Texture t_characters_bonus_icon[2][2];
extern sf::Texture t_characters_skill[2][4];
extern sf::Sprite s_characters_skill[2][4];
extern sf::Texture t_pet[2];

extern sf::Texture t_stats_window;
extern sf::Texture t_pause_menu_window;
extern sf::Texture t_dialog_window;
extern sf::Texture t_heart;
extern sf::Texture t_experience_bar;
extern sf::Texture t_discount_sign;
extern sf::Texture t_button;
extern sf::Texture t_checkbox;
extern sf::Texture t_profile_button;
extern sf::Texture t_button_rebirth;
extern sf::Texture t_button_extra_life;
extern sf::Texture t_button_extra_hp;
extern sf::Texture t_close_button;
extern sf::Texture t_selected_character;

bool initGraph();
bool shutdownGraph();

#endif //!graphics_h