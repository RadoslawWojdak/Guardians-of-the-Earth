#ifndef bonusblock_h
#define bonusblock_h

#include "Box2D\Box2D.h"
#include "block.h"
#include "treasure.h"
#include "enums.h"

class cBonusBlock :public cBlock
{
	short min_treasures, max_treasures;	//Mo¿liwa iloœæ skarbów wypadaj¹cych z bloku po jego zniszczeniu

public:
	cBonusBlock(b2World *physics_world, sf::Texture &texture, sf::Vector2f, short min_treasures = 1, short max_treasures = 4);
	
	void dropTreasures(b2World *physics_world, eWorld world_type, std::vector <cTreasure> &treasure, sf::Vector2f hit_force);
};

#endif //!bonusblock_h