#ifndef npc_h
#define npc_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

struct sFeatures
{
	bool friendly;		//Czy NPC bêdzie zaprzyjaŸniony z postaciami gracza? (nie bêdzie ich rani³, ani one jego)
	bool top_hurts;		//Czy Górna czêœæ NPC-a bêdzie raniæ postacie gracza?

	bool motion;		//Czy NPC bêdzie siê poruszaæ?
	bool chase;			//Czy NPC bêdzie goni³ postacie graczy?
	float max_speed;	//Maksymalna prêdkoœæ NPC (w przypadku chase prêdkoœæ nie jest sta³a)
	
	bool flying;		//Czy NPC bêdzie móg³ lataæ?
	bool swimming;		//Czy NPC bêdzie móg³ p³ywaæ?
	bool jumping;		//Czy NPC bêdzie podskakiwaæ?
};

unsigned short randomNPCID(eWorld world_type);

class cNPC :public cObjectLevel
{
	unsigned short id;	//ID NPC-A (NUMER DO£¥CZONY DO NPC-A - W NAZWIE PLIKU)
	eDirection dir;		//Kierunek w którym porusza siê postaæ
	bool start = false;	//Czy NPC zacz¹³ siê ju¿ poruszaæ? (Zaczyna dopiero, gdy w okolicy jest gracz i porusza siê a¿ do swojej œmierci lub zakoñczenia rozgrywki)
	
	b2BodyDef body_def;
	b2Body *body;

	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p³ynie obiekt jest aktualnie zanurzony
	bool dead;

	//CECHY NPC-ÓW
	float hp;
	sFeatures features;

	float speed;		//Prêdkoœæ NPC
	b2Vec2 last_speed;	//Poprzednia prêdkoœæ NPC (pionowa i pozioma)
	sf::Vector2f last_position;

	
	void setFeatures(unsigned short id);

public:
	cNPC(b2World &physics_world, eWorld world_type, bool *modulators, unsigned short id, sf::Vector2f pos, eDirection direction);

	void step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab, sf::FloatRect &view_rect);
	void hurt(float strength);
	void kill();

	void setAllPositions(sf::Vector2f pos);

	b2Body* getBody();
	sFeatures getFeatures();
	unsigned short getExperience();
	sf::Vector2f getLastPosition();
	bool isDead();
};

#endif //!npc_h