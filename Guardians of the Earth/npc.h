#ifndef npc_h
#define npc_h

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "objectlevel.h"
#include "graphics.h"
#include "enums.h"

struct sFeatures
{
	bool friendly;		//Czy NPC b�dzie zaprzyja�niony z postaciami gracza? (nie b�dzie ich rani�, ani one jego)
	bool top_hurts;		//Czy G�rna cz�� NPC-a b�dzie rani� postacie gracza?

	bool motion;		//Czy NPC b�dzie si� porusza�?
	bool chase;			//Czy NPC b�dzie goni� postacie graczy?
	float max_speed;	//Maksymalna pr�dko�� NPC (w przypadku chase pr�dko�� nie jest sta�a)
	
	bool flying;		//Czy NPC b�dzie m�g� lata�?
	bool swimming;		//Czy NPC b�dzie m�g� p�ywa�?
	bool jumping;		//Czy NPC b�dzie podskakiwa�?
};

unsigned short randomNPCID(eWorld world_type);

class cNPC :public cObjectLevel
{
	unsigned short id;	//ID NPC-A (NUMER DO��CZONY DO NPC-A - W NAZWIE PLIKU)
	eDirection dir;		//Kierunek w kt�rym porusza si� posta�
	bool start = false;	//Czy NPC zacz�� si� ju� porusza�? (Zaczyna dopiero, gdy w okolicy jest gracz i porusza si� a� do swojej �mierci lub zako�czenia rozgrywki)
	
	b2BodyDef body_def;
	b2Body *body;

	eFluidType is_immersed_in;	//Zmienna odpowiedzialna za sprawdzanie, w jakim p�ynie obiekt jest aktualnie zanurzony
	bool dead;

	//CECHY NPC-�W
	float hp;
	sFeatures features;

	float speed;		//Pr�dko�� NPC
	b2Vec2 last_speed;	//Poprzednia pr�dko�� NPC (pionowa i pozioma)
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