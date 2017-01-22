#ifndef global_variables_h
#define global_variables_h

#include "SFML\Graphics.hpp"

const int g_number_of_characters = 4;
const int g_scoreboard_size = 20;

const int g_unlocked_types = 1;	//Typy mo¿liwych do odblokowania przedmiotów (modulatory, NPC-y, itd.)
const int g_unlocked_modulators = 2;	//Iloœæ mo¿liwych do odblokowania modulatorów
const int g_all_modulators = 6 + g_unlocked_modulators;

extern unsigned int g_width, g_height;	//Wysokoœæ i szerokoœæ okna
extern bool g_fullscreen;	//Czy jest w³¹czony tryb pe³noekranowy
extern int g_framerate_limit;	//Czêstotliwoœæ odœwie¿ania
extern float g_g;	//Si³a grawitacji
extern float g_score_multipler;
//Struktura zawieraj¹ca mno¿nik prêdkoœci obiektów, które siê w niej znajduj¹
struct sFluidSpeedMultipler
{
	float water = 0.5f;
	float quicksand = 0.2f;
	float ice = 0.0f;
};
extern sFluidSpeedMultipler g_fluid_speed_multipler;

union uButton
{
	sf::Keyboard::Key key;
	unsigned int button;
};

struct sControlKeys
{
	bool is_pad;
	unsigned int pad;

	uButton up;
	uButton down;
	uButton left;
	uButton right;
	uButton jump;
	uButton fire;
	uButton special1;
};

extern sControlKeys g_key[4];

void initControlKeys();

#endif global_variables_h