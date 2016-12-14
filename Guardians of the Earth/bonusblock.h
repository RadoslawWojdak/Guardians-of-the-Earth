#ifndef bonusblock_h
#define bonusblock_h

#include "Box2D\Box2D.h"
#include "block.h"
#include "treasure.h"
#include "enums.h"

class cBonusBlock :public cBlock
{
	short min_treasures, max_treasures;	//Mo�liwa ilo�� skarb�w wypadaj�cych z bloku po jego zniszczeniu

public:
	cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f pos);
	
	void dropTreasures(b2World *physics_world, eWorld world_type, std::vector <cTreasure> &treasure);

	b2Body* getBody();
};

#endif //!bonusblock_h