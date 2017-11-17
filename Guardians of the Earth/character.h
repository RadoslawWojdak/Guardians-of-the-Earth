#ifndef character_h
#define character_h

#include <sstream>	//Konwersja liczby na tekst
#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include "characterAnimation.h"
#include "enums.h"
#include "graphics.h"
#include "treasure.h"
#include "fonts.h"
#include "fluid.h"
#include "npc.h"
#include "trampoline.h"
#include "ladder.h"
#include "bonusblock.h"
#include "pet.h"
#include "global_variables.h"
#include "powerup.h"
#include "bullet.h"
#include "button.h"

class cBullet;

class cCharacter :public cCharacterAnimation
{
protected:
	b2BodyDef body_def;
	b2Body *body;

	eCharacter character_type;
	short player_no;
	eDirection dir = DIR_RIGHT;

	sf::Sprite exp_bar;
	unsigned short lvl = 1;
	unsigned int exp = 0;
	unsigned short skill_points = 0;
	unsigned short number_of_skill[4] = {};

	cPet pet;
	sf::Vector2f pet_point;	//Punkt do którego bêdzie lata³ pet (wskaŸnik ze wzglêdu na to, ¿e gdy wstawia siê postaæ z temp_character do tablicy wektorowej character, to za ka¿dym razem zmienia³ siê adres)

	b2Vec2 last_speed;
	bool fire = true;		//Czy gracz ca³y czas trzyma klawisz strza³u?
	bool can_crush;			//Czy postaæ gracza mo¿e zgniataæ NPC-y?
	bool can_jump = false;
	bool stop_jump = true;		//Czy gracz ca³y czas trzyma naciœniêt¹ spacjê? (dziêki temu postaæ mo¿e skakaæ na ró¿ne wysokoœci)

	sf::Sprite stats_window;
	sf::Sprite heart;
	sf::Sprite taser;
	sf::Sprite bonus_sprite[2];
	sf::Sprite magic_shield;
	unsigned int immunity_time;
	unsigned int magic_shield_time;
	unsigned int special1_time;
	unsigned short life;
	unsigned int score;
	unsigned int cash;
	bool has_taser;

	float max_speed_x;
	float extra_speed;
	unsigned short extra_jump;	//Iloœæ dodatkowych skoków
	float extra_height_of_jump;
	unsigned short possible_extra_jumps = 0;	//Aktualna iloœæ mo¿liwych dodatkowych skoków do wykonania
	sf::Vector2f last_position;

	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p³ynie obiekt jest aktualnie zanurzony
	bool is_on_ice;
	bool is_on_ladder;
	bool dead;

	//Bonusy
	unsigned int bonus[2];	//Jakoœæ bonusu (ulepszenie lub iloœæ (np. pocisków))
	eDirection shot_dir;

	void jump(float force);
	void levelUp();
	void startInviolability();
	void immunityCountdown();
	void magicShieldCountdown();
	void turnOffMagicShield();

public:
	cCharacter(b2World &physics_world, eWorld world_type, sf::Vector2f pos, short player_no, bool *modulators);

	//S³u¿y do wczytywania wartoœci, które s¹ wspólne dla wszystkich klas dziedzicz¹cych
	void loadParameters(unsigned short lvl, unsigned int exp, unsigned short skill_point, unsigned short number_of_skill[4], unsigned short pet_hp, unsigned int bonus[2], unsigned short life, unsigned int score, unsigned int cash, bool has_taser);

	//S³u¿y tylko do wczytywania pewnych wartoœci, a nie tworzenia ca³ej klasy
	virtual void loadCharacter(unsigned short lvl, unsigned int exp, unsigned short skill_point, unsigned short number_of_skill[4], unsigned short pet_hp, unsigned int bonus[2], unsigned short life, unsigned int score, unsigned int cash, bool has_taser);

	void bodyRecreate(b2World &physics_world, eWorld world_type);	//Nadaje postaci ponownie cia³o

	void initPet();	//Inicjalizacja pet'a powinna siê odbywaæ przy ka¿dym utworzeniu obiektu klasy cCharacter (chyba ¿e jest to obiekt tymczasowy)

	void beenHit();
	void kill();
	virtual void control(b2World &physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	virtual void specialCollisions(b2World &physics_world, eWorld world_type, bool *modulators, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block);	//Wszystkie kolizje spoza œwiata Box2D (kolizje oparte o grafikê SFML)
	void applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size);
	void move(sf::RenderWindow &win, sf::Vector2f level_size);
	virtual void checkIndicators(b2World &world, eWorld world_type, std::vector <cCharacter*> player, std::vector <cBullet> &bullet);	//Sprawdzenie wskaŸników takich jak timery i punkty doœwiadczenia
	void rebirth();

	void addHP();
	void addLife();
	virtual void addPower(short power_id);
	void useMagicShield(unsigned int time);

	void addStatsForPowerUp(cPowerUp &power_up);
	void addStatsForTreasure(cTreasure &treasure);
	void addStatsForNPC(cNPC &npc);
	void addStatsForBonusBlock();
	void addStatsForEndOfLevel(unsigned int level_number, unsigned short experience_countdown);
	virtual void addSkill(unsigned short skill_id);
	void subtractCash(unsigned int how_many_to_subtract);

	void draw(sf::RenderWindow &win);
	void drawStats(sf::RenderWindow &win, sf::Vector2f left_top_corner);
	virtual void drawSkillTree(sf::RenderWindow &win, sf::Vector2f left_top_corner, unsigned short selected_skill, bool close_pressed);

	void setAllPositions(sf::Vector2f pos);

	short getPlayerNo();
	cPet getPet();
	b2Body *getBody();
	bool hasLife();
	bool isPetAlive();
	bool isDead();
	bool isInviolability();
	bool hasMagicShield();
	bool isSpecial1();

	eCharacter getCharacterType();
	unsigned short getLife();
	unsigned short getHP();
	unsigned int getScore();
	unsigned int getCash();
	unsigned int getBonus(short bonus_id);
	unsigned short getLevel();
	unsigned int getExperience();
	unsigned short getSkillPoints();
	unsigned short getNumberOfSkill(short skill_id);
	unsigned int requiredExpToLevelUp();
	bool hasTaser();
};

#endif character_h
