#ifndef sorceress_h
#define sorceress_h

#include "character.h"

class cSorceress :public cCharacter
{
	void startShooting();
	bool isShooting();
	void startHealing();
	bool isHealing();
	void heal(std::vector <cCharacter*> player);
	
	virtual void addPower(short power_id);

public:
	cSorceress(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	void shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction);

	virtual void checkIndicators(b2World * world, eWorld world_type, std::vector <cCharacter*> player, std::vector<cBullet>& bullet);

	virtual void addSkill(unsigned short skill_id);
	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!sorceress_h