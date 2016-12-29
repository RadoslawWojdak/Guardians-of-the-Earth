#ifndef character_h
#define character_h

#include <sstream>	//Konwersja liczby na tekst
#include "SFML\Graphics.hpp";
#include "Box2D.h"
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

class cBullet;

union uButton
{
	sf::Keyboard::Key key;
	unsigned int button;
};

struct sControlKeys
{
	bool is_pad;
	unsigned int pad;

	uButton up;
	uButton down;
	uButton left;
	uButton right;
	uButton jump;
	uButton fire;
};

class cCharacter :public cCharacterAnimation
{
	b2BodyDef body_def;
	b2Body *body;

	short player_no;
	eDirection dir = DIR_RIGHT;

	cPet pet;
	sf::Vector2f pet_point;	//Punkt do kt�rego b�dzie lata� pet (wska�nik ze wzgl�du na to, �e gdy wstawia si� posta� z temp_character do tablicy wektorowej character, to za ka�dym razem zmienia� si� adres)

	b2Vec2 last_speed;
	bool fire = true;		//Czy gracz ca�y czas trzyma klawisz strza�u?
	bool can_jump = false;
	bool stop_jump = true;		//Czy gracz ca�y czas trzyma naci�ni�t� spacj�? (dzi�ki temu posta� mo�e skaka� na r�ne wysoko�ci)

	sControlKeys key;

	sf::Sprite stats_window;
	sf::Sprite heart;
	sf::Sprite bonus_sprite;
	unsigned int immunity_time;
	unsigned short life;
	unsigned int score;
	unsigned int cash;

	float max_speed_x;
	sf::Vector2f last_position;
	
	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p�ynie obiekt jest aktualnie zanurzony
	bool is_on_ice;
	bool is_on_ladder;
	bool dead;

	//Bonusy
	unsigned int bonus;	//Jako�� bonusu (ulepszenie lub ilo�� (np. pocisk�w))

	void initControlKeys(short player_no);
	void jump(float force);
	void shot(b2World *world, eWorld world_type, std::vector <cBullet> &bullet);
	void startInviolability();
	void immunityCountdown();

public:
	cCharacter(b2World *physics_world, eWorld world_type, sf::Vector2f pos, short player_no);

	void bodyRecreate(b2World &physics_world, eWorld world_type);	//Nadaje postaci ponownie cia�o

	void initPet();	//Inicjalizacja pet'a powinna si� odbywa� przy ka�dym utworzeniu obiektu klasy cCharacter (chyba �e jest to obiekt tymczasowy)
	
	void beenHit();
	void kill();
	void control(b2World *physics_world, eWorld world_type, std::vector <cBullet> &bullet);
	void specialCollisions(b2World *physics_world, eWorld world_type, std::vector <cNPC> &npc, std::vector <cPowerUp> &power_up, std::vector <cTreasure> &treasure, std::vector <cFluid> &fluid, std::vector <cTrampoline> &trampoline, std::vector <cLadder> &ladder, std::vector <cBonusBlock> &bonus_block);	//Wszystkie kolizje spoza �wiata Box2D (kolizje oparte o grafik� SFML)
	void applyPhysics(eWorld world_type, bool *fluid, sf::Vector2i grid_size);
	void move(sf::RenderWindow &win, sf::Vector2f level_size);
	void rebirth();

	void addHP();
	void addLife();

	void addStatsForPowerUp(cPowerUp &power_up);
	void addStatsForTreasure(cTreasure &treasure);
	void addStatsForNPC(cNPC &npc);
	void addStatsForBonusBlock();
	void subtractCash(unsigned int how_many_to_subtract);
	void drawStats(sf::RenderWindow &win, sf::Vector2f left_top_corner);

	void setAllPositions(sf::Vector2f pos);
	
	cPet getPet();
	b2Body *getBody();
	sControlKeys getControlKeys();
	bool hasLife();
	bool isPetAlive();
	bool isDead();
	bool isInviolability();
	unsigned int getCash();
};

#endif character_h