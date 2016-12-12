#ifndef bonusblock_h
#define bonusblock_h

#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "treasure.h"
#include "enums.h"

class cBonusBlock :public cObjectLevel
{
	b2BodyDef body_def;
	b2Body *body;

	short min_treasures, max_treasures;	//Mo¿liwa iloœæ skarbów wypadaj¹cych z bloku po jego zniszczeniu
	//std::vector <cTreasure> *treasure;

public:
	cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos/*, std::vector <cTreasure> *treasure*/);
	
	void dropTreasures(b2World *physics_world, eWorld world_type, std::vector <cTreasure> &treasure);

	b2Body* getBody();
};

#endif //!bonusblock_h