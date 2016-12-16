#ifndef character_h
#define character_h

#include "SFML\Graphics.hpp";
#include "Box2D.h"
#include "objectlevel.h"
#include "enums.h"
#include "graphics.h"
#include "treasure.h"

struct sControlKey
{
	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key left;
	sf::Keyboard::Key right;
	sf::Keyboard::Key jump;
	sf::Keyboard::Key fire;
};

class cCharacter :public cObjectLevel
{
	b2BodyDef body_def;
	b2Body *body;

	b2Vec2 last_speed;
	bool can_jump = false;

	sControlKey key;

	unsigned short life;
	unsigned int score;
	unsigned int cash;

	float max_speed_x;

	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p³ynie obiekt jest aktualnie zanurzony

	void addStatsForTreasure(cTreasure &treasure);

public:
	cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos);

	void control();
	void specjalCollisions(std::vector <cTreasure> &treasure);	//kolizje nie wp³ywaj¹ce na fizykê postaci (np. kolizje z monetami, NPC-ami, trampolinami)
	void applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size);
	void move();

	void setAllPositions(sf::Vector2f pos);
};

#endif character_h