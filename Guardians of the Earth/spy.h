#ifndef spy_h
#define spy_h

#include "character.h"

class cSpy :public cCharacter
{
	short ivisibility_timer;

	void startShooting();
	bool isShooting();
	void startInvisibility();
	void invisibilityCountdown();
	void stopInvisibility();
	bool isInvisibility();

	virtual void addPower(short power_id);

public:
	cSpy(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	void shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction);

	virtual void checkIndicators(b2World * world, eWorld world_type, std::vector<cBullet>& bullet);

	virtual void addSkill(unsigned short skill_id);
	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!spy_h