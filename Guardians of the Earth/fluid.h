#ifndef fluid_h
#define fluid_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

void fluidInit(b2World *physics_world);

class cFluid :public cObjectLevel
{
	//eFluid fluid_type;	//Typ p造nu

	//Wszystkie p造ny jako jedno cia這 (w celu optymalizacji)
	static unsigned int fluid_id;
	static b2BodyDef physics_body_def;
	static b2Body *physics_body;
	//!Wszystkie p造ny jako jedno cia這

public:
	cFluid();		//Konstruktor przeznaczony tylko do inicjalizacji
	cFluid(eWorld world_type, sf::Vector2f pos);

	void graphicsCustomize(eWorld world_type, sf::Vector2u map_size, bool *another_object_exists, sf::Vector2i grid_size);	//another_object_exists - ka盥y obiekt kt鏎 sprawia, 瞠 woda b璠zie ca趾owicie zape軟iona

	void resetMainParameters(b2World *physics_world);	//Resetowanie parametr闚 nale蕨cych do wszystkich grunt闚 (statycznych)
};

#endif //!fluid_h