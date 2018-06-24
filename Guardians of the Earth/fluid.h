#ifndef fluid_h
#define fluid_h

#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

void fluidInit(b2World &physics_world);

class cFluid :public cObjectLevel
{
	//Wszystkie p³yny jako jedno cia³o (w celu optymalizacji)
	static unsigned int fluid_id;
	static b2BodyDef physics_body_def;
	static b2Body *physics_body;
	//!Wszystkie p³yny jako jedno cia³o

public:
	cFluid();		//Konstruktor przeznaczony tylko do inicjalizacji p³ynów
	cFluid(eWorld world_type, sf::Vector2f pos);

	void graphicsCustomize(eWorld world_type, sf::Vector2u map_size, bool *another_object_exists, sf::Vector2i grid_size);	//another_object_exists - ka¿dy obiekt któr sprawia, ¿e woda bêdzie ca³kowicie zape³niona

	void resetMainParameters(b2World &physics_world);	//Resetowanie parametrów nale¿¹cych do wszystkich gruntów (statycznych)

	b2Body *getBody();
};

#endif //!fluid_h
