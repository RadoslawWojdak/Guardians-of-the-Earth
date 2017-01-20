#ifndef spy_h
#define spy_h

#include "character.h"

class cSpy :public cCharacter
{
	short ivisibility_timer;
	short taser_timer;		//czas do mo¿liwego ponownego u¿ycia paralizatora

	void startShooting();
	bool isShooting();
	void startInvisibility();
	void invisibilityCountdown();
	void stopInvisibility();
	bool isInvisibility();

	void usedTaser();
	void taserCountdown();
	bool canUseTaser();

	virtual void addPower(short power_id);

public:
	cSpy(b2World &physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	virtual void control(b2World &physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	virtual void specialCollisions(b2World &physics_world, eWorld world_type, bool *modulators, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block);	//Wszystkie kolizje spoza œwiata Box2D (kolizje oparte o grafikê SFML)
	void shot(b2World &world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction);

	virtual void checkIndicators(b2World & world, eWorld world_type, std::vector <cCharacter*> player, std::vector<cBullet>& bullet);

	virtual void addSkill(unsigned short skill_id);
	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!spy_h