#ifndef archer_h
#define archer_h

#include "character.h"

class cArcher :public cCharacter
{
	unsigned short arrows;

	short extra_shot_timer;
	short bonus2_timer;

	void extraShotCountdown();
	void bonus2Countdown();
	void startShooting();
	bool isShooting();

	virtual void addPower(short power_id);

public:
	cArcher(b2World &physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	//S³u¿y tylko do wczytywania pewnych wartoœci, a nie tworzenia ca³ej klasy
	virtual void loadCharacter(unsigned short lvl, unsigned int exp, unsigned short skill_point, unsigned short number_of_skill[4], unsigned short pet_hp, unsigned int bonus[2], unsigned short life, unsigned int score, unsigned int cash, bool has_taser);

	virtual void control(b2World &physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	void shot(b2World &world, eWorld world_type, std::vector <cBullet> &bullet, eDirection shot_direction, float strength = 0.25f, unsigned short piercing = 1);

	virtual void checkIndicators(b2World & world, eWorld world_type, std::vector <cCharacter*> player, std::vector<cBullet>& bullet);

	virtual void addSkill(unsigned short skill_id);
	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);
};

#endif //!archer_h