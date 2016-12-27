#ifndef bullet_h
#define bullet_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"
#include "global_variables.h"
#include "npc.h"
#include "fluid.h"
#include "bonusblock.h"
#include "treasure.h"
#include "character.h"

class cCharacter;

class cBullet :public cObjectLevel
{
protected:
	b2BodyDef body_def;
	b2Body *body;

private:
	bool destroyed = false;
	bool gravity;
	sf::Vector2f last_pos;
	b2Vec2 speed;
	short player_id;	//Do jakiego gracza nale�y (0 = NPC)

public:
	cBullet(b2World *physics_world, eWorld world_type, b2Vec2 speed, sf::Vector2f pos, short player_id = 0);

	void step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab);
	void specialCollisions(b2World *physics_world, eWorld world_type, std::vector <cCharacter> &character, std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cBonusBlock> &bonus_block);

	b2Body *getBody();

	bool isDestroyed();
};

#endif //!bullet_h