#ifndef character_h
#define character_h

#include <sstream>	//Konwersja liczby na tekst
#include "SFML\Graphics.hpp";
#include "Box2D.h"
#include "objectlevel.h"
#include "enums.h"
#include "graphics.h"
#include "treasure.h"
#include "fonts.h"
#include "fluid.h"
#include "npc.h"

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
	sf::Vector2f last_position;
	
	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p³ynie obiekt jest aktualnie zanurzony
	bool is_on_ice;
	bool dead;

	void addStatsForTreasure(cTreasure &treasure);
	void addStatsForNPC(cNPC &npc);
	void jump(float force);

public:
	cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos);

	void kill();
	void control();
	void specjalCollisions(eWorld world_type, std::vector <cNPC> &npc, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid);	//Wszystkie kolizje spoza œwiata Box2D (kolizje oparte o grafikê SFML)
	void applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size);
	void move(sf::Vector2f level_size);

	void drawStats(sf::RenderWindow &win, sf::Vector2f left_top_corner);

	void setAllPositions(sf::Vector2f pos);
	bool isDead();
};

#endif character_h