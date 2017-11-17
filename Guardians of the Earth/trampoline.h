#ifndef trampoline_h
#define trampoline_h

#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"
#include "ground.h"
#include "block.h"
#include "bonusblock.h"

class cTrampoline :public cObjectLevel
{
	b2BodyDef body_def;
	b2Body *body;

	float force;	//Si³a z jak¹ wybija postacie graczy / NPC-y(?)

public:
	cTrampoline(b2World &physics_world, unsigned short id, sf::Vector2f pos, float force);

	b2Body *getBody();
};

#endif //!trampoline_h
