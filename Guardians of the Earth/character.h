#ifndef character_h
#define character_h

#include "SFML\Graphics.hpp";
#include "Box2D.h"
#include "objectlevel.h"
#include "enums.h"
#include "graphics.h"
#include "treasure.h"

class cCharacter :public cObjectLevel
{
	b2BodyDef body_def;
	b2Body *body;

	b2Vec2 last_speed;
	bool can_jump = false;
	unsigned short life;
	unsigned int score;
	unsigned int cash;

public:
	cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos);

	void control();
	void specjalCollisions(std::vector <cTreasure> &treasure);	//kolizje - interakcje (np. kolizje z monetami, NPC-ami, trampolinami)

	void addStatsForTreasure(cTreasure &treasure);

	void setAllPositions(sf::Vector2f pos);
};

#endif character_h