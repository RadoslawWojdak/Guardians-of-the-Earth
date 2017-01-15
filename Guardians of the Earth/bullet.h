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
	bool destroyed;
	bool stop;
	bool gravity;
	sf::Vector2f last_pos;
	b2Vec2 speed;
	b2Vec2 last_speed;
	short player_id;	//Do jakiego gracza nale¿y (0 = NPC)
	float strength;		//Ile hp odbierze NPC-owi
	unsigned short piercing;	//Ilu przeciwników mo¿e zabiæ, zanim zniknie (W przypadku NPC-ów z wiêksz¹ iloœci¹ HP odejmuje tyle, ile NPC ma HP)

public:
	cBullet(b2World *physics_world, eWorld world_type, sf::Texture &texture, bool gravity, b2Vec2 speed, sf::Vector2f pos, float strength, unsigned short piercing, short player_id = 0);

	void step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab);
	void specialCollisions(b2World *physics_world, eWorld world_type, bool *modulators, std::vector <cCharacter*> &character, std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cBonusBlock> &bonus_block);

	b2Body *getBody();

	bool isDestroyed();
};

#endif //!bullet_h