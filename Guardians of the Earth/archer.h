#ifndef archer_h
#define archer_h

#include "character.h"

class cArcher :public cCharacter
{
	void startShooting();
	bool isShooting();

public:
	cArcher(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	virtual void shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction);

	virtual void checkIndicators(b2World * world, eWorld world_type, std::vector<cBullet>& bullet);

	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!archer_h