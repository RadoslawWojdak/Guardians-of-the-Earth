#ifndef knight_h
#define knight_h

#include "character.h"

class cKnight :public cCharacter
{
	//Bonusy
	short b1_in_b2_timer;
	unsigned short bonus1_in_bonus2;	//Jak wiele pocisków ma jeszcze wystrzeliæ gracz, gdy jest w trakcie dzia³ania bonusu2

	void startSpecial1();
	void special1Countdown();
	void startB1InB2();
	void b1InB2Countdown(b2World *world, eWorld world_type, std::vector <cBullet> &bullet);

public:
	cKnight(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	virtual void shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction);
	
	virtual void checkIndicators(b2World * world, eWorld world_type, std::vector<cBullet>& bullet);

	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!knight_h