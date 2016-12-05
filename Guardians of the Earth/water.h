#ifndef water_h
#define water_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

void waterInit(b2World *physics_world);

class cWater :public cObjectLevel
{
	//Wszystkie wody jako jedno cia³o (w celu optymalizacji)
	static unsigned int water_id;
	static b2BodyDef physics_body_def;
	static b2Body *physics_body;
	//!Wszystkie wody jako jedno cia³o

public:
	cWater();		//Przeznaczony tylko do groundsInit
	cWater(sf::Texture &texture, sf::Vector2f pos);

	void graphicsCustomize(sf::Vector2u map_size, std::vector <cWater> &water);

	void resetMainParameters(b2World *physics_world);	//Resetowanie parametrów nale¿¹cych do wszystkich gruntów (statycznych)
};

#endif //!water_h