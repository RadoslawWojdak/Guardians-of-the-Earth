#ifndef treasure_h
#define treasure_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <time.h>
#include "graphics.h"
#include "objectlevel.h"
#include "global_variables.h"
#include "enums.h"

/*
-1 - dodatkowe ¿ycie
*/

class cTreasure :public cObjectLevel
{
	b2BodyDef body_def;
	b2Body *body;

	short value;	//Wartoœæ skarbu; wartoœci ujemne okreœlaj¹ skarb o innych cechach (dodatkowe ¿ycie, itp.)
	
	bool physics;	//Czy na skarb dzia³a fizyka? (ruch, grawitacja, itd.)
	//sf::Vector2f speed;	//Prêdkoœæ poruszania sie obiektu

public:
	cTreasure(b2World *physics_world, eWorld world_type, sf::Vector2f pos, bool physics = false, float speed_x = 0, float speed_y = 0);

	void step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab);
};

#endif //!treasure_h