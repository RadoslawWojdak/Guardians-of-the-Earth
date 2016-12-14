#ifndef ground_h
#define ground_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "graphics.h"
#include "objectlevel.h"
#include "enums.h"

void groundsInit(b2World *physics_world);

class cGround :public cObjectLevel
{
	eWorld world_type;
	
	//Wszystkie grunty jako jedno cia³o (w celu optymalizacji)
	static unsigned int ground_id;
	static b2BodyDef physics_body_def;
	static b2Body *physics_body;
	//!Wszystkie grunty jako jedno cia³o

public:
	cGround();		//Przeznaczony tylko do groundsInit
	cGround(sf::Vector2f pos, eWorld world);

	void graphicsCustomize(sf::Vector2u map_size, bool *ground_exists, sf::Vector2i grid_size);

	void resetMainParameters(b2World *physics_world);	//Resetowanie parametrów nale¿¹cych do wszystkich gruntów (statycznych)
};

#endif //!ground_h