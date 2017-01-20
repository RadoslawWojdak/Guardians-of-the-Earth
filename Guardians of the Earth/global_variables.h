#ifndef global_variables_h
#define global_variables_h

const int g_number_of_characters = 4;

extern unsigned int g_width, g_height;	//Wysoko�� i szeroko�� okna
extern bool g_fullscreen;	//Czy jest w��czony tryb pe�noekranowy
extern int g_framerate_limit;	//Cz�stotliwo�� od�wie�ania
extern float g_g;	//Si�a grawitacji
extern float g_score_multipler;
//Struktura zawieraj�ca mno�nik pr�dko�ci obiekt�w, kt�re si� w niej znajduj�
struct sFluidSpeedMultipler
{
	float water = 0.5f;
	float quicksand = 0.2f;
	float ice = 0.0f;
};
extern sFluidSpeedMultipler g_fluid_speed_multipler;

#endif global_variables_h