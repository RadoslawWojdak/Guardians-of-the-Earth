#ifndef knight_h
#define knight_h

#include "character.h"

class cKnight :public cCharacter
{
public:
	cKnight(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
};

#endif //!knight_h