#ifndef global_variables_h
#define global_variables_h

const int g_number_of_characters = 4;

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

#endif global_variables_h