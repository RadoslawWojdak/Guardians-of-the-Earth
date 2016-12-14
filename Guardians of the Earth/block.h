#ifndef block_h
#define block_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "graphics.h"
#include "objectlevel.h"
#include "enums.h"

class cBlock :public cObjectLevel
{
protected:
	b2BodyDef body_def;
	b2Body *body;

public:
	cBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos);
};

#endif //!block_h