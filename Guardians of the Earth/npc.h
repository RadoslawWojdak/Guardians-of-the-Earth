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

	bool motion;		//Czy NPC bêdzie siê poruszaæ?
	bool chase;			//Czy NPC bêdzie goni³ postacie graczy?
	float max_speed;	//Maksymalna prêdkoœæ NPC (w przypadku chase prêdkoœæ nie jest sta³a)

	bool flying;		//Czy NPC bêdzie móg³ lataæ?
	bool swimming;		//Czy NPC bêdzie móg³ p³ywaæ?
	bool jumping;		//Czy NPC bêdzie podskakiwaæ?
};

class cNPC :public cObjectLevel
{
	unsigned short id;	//ID NPC-A (NUMER DO£¥CZONY DO NPC-A - W NAZWIE PLIKU)
	eDirection dir;		//Kierunek w którym porusza siê postaæ
	bool start = false;	//Czy NPC zacz¹³ siê ju¿ poruszaæ? (Zaczyna dopiero, gdy w okolicy jest gracz i porusza siê a¿ do swojej œmierci lub zakoñczenia rozgrywki)
	
	b2BodyDef body_def;
	b2Body *body;
	
	//CECHY NPC-ÓW
	sFeatures features;

	float speed;		//Prêdkoœæ NPC
	b2Vec2 last_speed;	//Poprzednia prêdkoœæ NPC (pionowa i pozioma)
	sf::Vector2f last_position;

	
	void setFeatures(unsigned short id);

public:
	cNPC(b2World *physics_world, eWorld world_type, unsigned short id, sf::Vector2f pos, eDirection direction);

	void step(eWorld world_type, sf::Vector2i world_size, bool *fluid_tab, sf::FloatRect &view_rect);//std::vector <cGround> &ground, std::vector <cBlock> &block, std::vector <cBonusBlock> &bonus_block, std::vector <cNPC> &npc);
	//void startMoving();

	void setAllPositions(sf::Vector2f pos);
	void moveAllPositions(sf::Vector2f pos);

	//U¯YWAÆ TYLKO PRZY GENEROWANIU MAPY!
	bool isSolidCollision(bool *solid_object, bool *npc_exists, sf::Vector2i grid_size);	//Sprawdzanie czy NPC koliduje z jakimœ cia³em sztywnym (gruntem, blokiem, bonusowym blokiem, innym NPC, ...), grid_size - rozmiary siatki (szerokoœæ i wysokoœæ mapy podzielona przez 32)

	b2Body* getBody();
	sFeatures getFeatures();
};

#endif //!npc_h