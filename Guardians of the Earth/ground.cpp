#include "ground.h"
#include <iostream>
unsigned int cGround::ground_id = 0;
b2BodyDef cGround::physics_body_def;
b2Body *cGround::physics_body = NULL;

void groundsInit(b2World *physics_world)
{
	cGround gr;
	gr.resetMainParameters(physics_world);
};

cGround::cGround()
{
	;
}

cGround::cGround(sf::Vector2f pos, eWorld world)
{
	this->world_type = world;
	this->adjustGraphicsParameters(t_ground[world][0], pos);

	//BOX2D
	b2ChainShape shape;		//Przy wype³nionym wieloko¹cie jest masa b³êdów dotycz¹cych kolizji
	const float32 x = pos.x * 0.02f;
	const float32 y = pos.y * 0.02f;

	b2Vec2 vs[4];
	vs[0].Set(x + 0.0f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[1].Set(x + 32 * 0.02f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[2].Set(x + 32 * 0.02f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);
	vs[3].Set(x + 0.0f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);
	
	shape.CreateLoop(vs, 4);
	//shape.SetAsBox(0.02f * 32.0f / 2.0f, 0.02f * 32.0f / 2.0f, b2Vec2(x, y), 0.0f);
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_GROUND);
	fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE);
	fd.friction = 0.0f;
	this->physics_body->CreateFixture(&fd);

	this->ground_id++;

	/*for (int i = 0; i < 4; i++)
		std::cout << shape.GetVertex(i).x << ":" << shape.GetVertex(i).y << " ; ";
	std::cout << "\n";
	*/

	
	//std::cout << this->physics_body->GetFixtureList()->GetShape()->
	//std::cout << this->physics_body->GetFixtureList()->GetAABB(0). << ".....\n";
}

/*void cGround::bodyCustomize(bool tab_ground[][], int x, int y)
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			if (tab_ground[j][i])
			{
				sf::Vector2f pos;
				pos.x = j;
				pos.y = i;

				do
				{

				}
			}
		}
	}
}*/

void cGround::graphicsCustomize(sf::Vector2u map_size, bool *ground_exists, sf::Vector2i grid_size)
{
	//Pozycja ziemii
	int x = getPosition().x / 32;
	int y = getPosition().y / 32 - 1;

	//Bool - czy z tej strony bloku znajduje sie inny blok
	/*
	012
	3 4
	567
	*/
	bool exists[8] = {};

	//Dostosowywanie grafiki bloku do reszty bloków
	
	//DOSTOSOWYWANIE ZE WZGLÊDU NA KONIEC POZIOMU
	if (x == 0)
	{
		exists[0] = true;
		exists[3] = true;
		exists[5] = true;
	}
	else if (x == grid_size.x - 1)
	{
		exists[2] = true;
		exists[4] = true;
		exists[7] = true;
	}
	if (y == 0)
	{
		exists[0] = true;
		exists[1] = true;
		exists[2] = true;
	}
	else if (y == grid_size.y - 1)
	{
		exists[5] = true;
		exists[6] = true;
		exists[7] = true;
	}
	//!DOSTOSOWYWANIE ZE WZGLÊDU NA KONIEC POZIOMU

	//DOSTOSOWYWANIE ZE WZGLÊDU NA INNE GRUNTY
	if (x != 0 && y != 0 && ground_exists[(y - 1) * grid_size.x + x - 1])	//GÓRA-LEWO
		exists[0] = true;
	if (y != 0 && ground_exists[(y - 1) * grid_size.x + x])	//GÓRA
		exists[1] = true;
	if (x != grid_size.x - 1 && y != 0 && ground_exists[(y - 1) * grid_size.x + x + 1])	//GÓRA-PRAWO
		exists[2] = true;
	if (x != 0 && ground_exists[y * grid_size.x + x - 1])	//LEWO
		exists[3] = true;
	if (x != grid_size.x - 1 && ground_exists[y * grid_size.x + x + 1])	//PRAWO
		exists[4] = true;
	if (x != 0 && y != grid_size.y - 1 && ground_exists[(y + 1) * grid_size.x + x - 1])	//DÓ£-LEWOO
		exists[5] = true;
	if (y != grid_size.y - 1 && ground_exists[(y + 1) * grid_size.x + x])	//DÓ£
		exists[6] = true;
	if (x != grid_size.x + 1 && y != grid_size.y + 1 && ground_exists[(y + 1) * grid_size.x + x + 1])	//GÓRA
		exists[7] = true;
	//!DOSTOSOWYWANIE ZE WZGLÊDU NA INNE GRUNTY
	/*{
		if (ground_exists[y * grid_size.x + x - 1])	//Lewo
			exists[3] = true;
	}
	else
		exists[3] = true;



	if (x > 0)	//Je¿eli po lewej stronie mog¹ znajdowaæ siê grunty
	{
		if (ground_exists[y * grid_size.x + x - 1])	//Lewo
			exists[3] = true;

		if (y > 0)	//Je¿eli ponad gruntem mog¹ znajdowaæ siê grunty
		{
			if (ground_exists[(y - 1) * grid_size.x + x - 1])	//Lewo-Góra
				exists[0] = true;
		}
		else	//Je¿eli powy¿ej jest koniec poziomu
			exists[0] = true;

		if (y < grid_size.y - 1)	//Je¿eli poni¿ej gruntu mog¹ znajdowaæ siê grunty
		{
			if (ground_exists[(y + 1) * grid_size.x + x - 1])	//Lewo-Dó³
				exists[5] = true;
		}
		else	//Je¿eli poni¿ej jest koniec poziomu
			exists[5];
	}
	else	//Je¿eli po lewej stronie jest koniec poziomu
		exists[3] = true;

	if (x < grid_size.x - 1)	//Je¿eli po prawej stronie mog¹ znajdowaæ siê grunty
	{
		if (ground_exists[y * grid_size.x + x + 1])	//Prawo
			exists[4] = true;

		if (y > 0)	//Je¿eli ponad gruntem mog¹ znajdowaæ siê grunty
		{
			if (ground_exists[(y - 1) * grid_size.x + x + 1])	//Prawo-Góra
				exists[2] = true;
		}
		else	//Je¿eli powy¿ej jest koniec poziomu
			exists[2] = true;

		if (y < grid_size.y - 1)	//Je¿eli poni¿ej gruntu mog¹ znajdowaæ siê grunty
		{
			if (ground_exists[(y + 1) * grid_size.x + x + 1])	//Prawo-Dó³
				exists[7] = true;
		}
		else	//Je¿eli poni¿ej jest koniec poziomu
			exists[7];
	}
	else	//Je¿eli po prawej stronie jest koniec poziomu
		exists[4] = true;

	if (y > 0)	//Je¿eli powy¿ej mog¹ znajdowaæ siê grunty
	{
		if (ground_exists[(y - 1) * grid_size.x + x])	//Góra
			exists[1] = true;
	}
	else	//Je¿eli powy¿ej jest koniec poziomu
		exists[1] = true;

	if (y < grid_size.y + 1)	//Je¿eli poni¿ej mog¹ znajdowaæ siê grunty
	{
		if (ground_exists[(y + 1) * grid_size.x + x])
			exists[6] = true;
	}
	else	//Je¿eli poni¿ej jest koniec poziomu
		exists[6] = true;*/


	/*for (int i = 0; i < ground.size(); i++)
	{
		int x2 = ground[i].getPosition().x / 32;
		int y2 = ground[i].getPosition().y / 32;

		if ((x - 1 == x2) && (y - 1 == y2))	//Lewo-Góra
			exists[0] = true;
		else if ((x == x2) && (y - 1 == y2))	//Góra
			exists[1] = true;
		else if ((x + 1 == x2) && (y - 1 == y2))	//Prawo-Góra
			exists[2] = true;
		else if ((x - 1 == x2) && (y == y2))	//Lewo
			exists[3] = true;
		else if ((x + 1 == x2) && (y == y2))	//Prawo
			exists[4] = true;
		else if ((x - 1 == x2) && (y + 1 == y2))	//Lewo-Dó³
			exists[5] = true;
		else if ((x == x2) && (y + 1 == y2))	//Dó³
			exists[6] = true;
		else if ((x + 1 == x2) && (y + 1 == y2))	//Prawo-Dó³
			exists[7] = true;

		if (x * 32 - 24 <= 0)
		{
			exists[0] = true;
			exists[3] = true;
			exists[5] = true;
		}
		else if ((x + 1) * 32 + 24 >= map_size.x)
		{
			exists[2] = true;
			exists[4] = true;
			exists[7] = true;
		}

		if (y * 32 - 24 <= 0)
		{
			exists[0] = true;
			exists[1] = true;
			exists[2] = true;
		}
		else if ((y + 1) * 32 + 24 >= map_size.y)
		{
			exists[5] = true;
			exists[6] = true;
			exists[7] = true;
		}
	}*/


	setTexture(t_ground[this->world_type][5]);		//Bez krawêdzi - domyœlny

	//Puste - Moga byc, ale nie musza
	//z "!" - Nie moga byc
	//bez "!" - Musza byc

	//ground 0
	if (/*(          )*/    (!exists[1]) &&   /*(          )*/
		(!exists[3]) && (!exists[4]) &&
		/*(          )*/    (!exists[6])        /*(          )*/)
	{
		setTexture(t_ground[this->world_type][0]);
	}
	//ground 1
	else if (/*(          )*/    (!exists[1]) &&   /*(          )*/
		(!exists[3]) && (exists[4]) &&
		/*(          )*/    (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][1]);
	}
	//ground 2
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][2]);
	}
	//ground 3
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		(exists[5]) && (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][3]);
	}
	//ground 4
	else if (/*(          )*/    (exists[1]) && (exists[2]) &&
		(!exists[3]) && (exists[4]) &&
		/*(          )*/    (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][4]);
	}
	//ground 6
	else if ((exists[0]) && (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		(exists[5]) && (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][6]);
	}
	//ground 7
	else if (/*(          )*/    (exists[1]) && (exists[2]) &&
		(!exists[3]) && (exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][7]);
	}
	//ground 8
	else if ((exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][8]);
	}
	//ground 9
	else if ((exists[0]) && (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][9]);
	}
	//ground 10
	else if ((exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][10]);
	}
	//ground 11
	else if ((exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][11]);
	}
	//ground 12
	else if ((exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][12]);
	}
	//ground 13
	else if ((!exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][13]);
	}
	//ground 14
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(!exists[3]) && (exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][14]);
	}
	//ground 15
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(exists[3]) && (exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][15]);
	}
	//ground 16
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][16]);
	}
	//ground 17
	else if (/*(          )*/    (!exists[1]) && /*(          )*/
		(!exists[3]) && (!exists[4]) &&
		/*(          )*/    (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][17]);
	}
	//ground 18
	else if (/*(          )*/    (exists[1]) && /*(          )*/
		(!exists[3]) && (!exists[4]) &&
		/*(          )*/    (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][18]);
	}
	//ground 19
	else if (/*(          )*/    (exists[1]) && /*(          )*/
		(!exists[3]) && (!exists[4]) &&
		/*(          )*/    (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][19]);
	}
	//ground 20
	else if (/*(          )*/  (exists[1]) && (!exists[2]) &&
		(!exists[3]) && (exists[4]) &&
		/*(          )*/  (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][20]);
	}
	//ground 21
	else if (/*(          )*/  (!exists[1]) && /*(          )*/
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][21]);
	}
	//ground 22
	else if ((!exists[0]) && (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		(!exists[5]) && (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][22]);
	}
	//ground 23
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		/*(          )*/  (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][23]);
	}
	//ground 24
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][24]);
	}
	//ground 25
	else if (/*(          )*/  (!exists[1]) && /*(          )*/
		(!exists[3]) && (exists[4]) &&
		/*(          )*/  (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][25]);
	}
	//ground 26
	else if (/*(          )*/  (!exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		/*(          )*/  (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][26]);
	}
	//ground 27
	else if (/*(          )*/  (exists[1]) && /*(          )*/
		(!exists[3]) && (exists[4]) &&
		/*(          )*/  (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][27]);
	}
	//ground 28
	else if (/*(          )*/  (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		/*(          )*/  (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][28]);
	}
	//ground 29
	else if (/*(          )*/  (!exists[1]) && /*(          )*/
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][29]);
	}
	//ground 30
	else if (/*(          )*/  (!exists[1]) && /*(          )*/
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][30]);
	}
	//ground 31
	else if (/*(          )*/  (exists[1]) && (exists[2]) &&
		(!exists[3]) && (exists[4]) &&
		/*(          )*/  (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][31]);
	}
	//ground 32
	else if (/*(          )*/  (exists[1]) && (!exists[2]) &&
		(!exists[3]) && (exists[4]) &&
		/*(          )*/  (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][32]);
	}
	//ground 33
	else if ((exists[0]) && (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		(!exists[5]) && (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][33]);
	}
	//ground 34
	else if ((!exists[0]) && (exists[1]) && /*(          )*/
		(exists[3]) && (!exists[4]) &&
		(exists[5]) && (exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][34]);
	}
	//ground 35
	else if ((exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		/*(          )*/  (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][35]);
	}
	//ground 36
	else if ((!exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		/*(          )*/  (!exists[6])      /*(          )*/)
	{
		setTexture(t_ground[this->world_type][36]);
	}
	//ground 37
	else if ((exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][37]);
	}
	//ground 38
	else if ((exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][38]);
	}
	//ground 39
	else if ((!exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][39]);
	}
	//ground 40
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][40]);
	}
	//ground 41
	else if ((exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][41]);
	}
	//ground 42
	else if ((!exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][42]);
	}
	//ground 43
	else if ((exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][43]);
	}
	//ground 44
	else if ((!exists[0]) && (exists[1]) && (exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][44]);
	}
	//ground 45
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(!exists[5]) && (exists[6]) && (exists[7]))
	{
		setTexture(t_ground[this->world_type][45]);
	}
	//ground 46
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) &&
		(exists[3]) && (exists[4]) &&
		(exists[5]) && (exists[6]) && (!exists[7]))
	{
		setTexture(t_ground[this->world_type][46]);
	}
}

void cGround::resetMainParameters(b2World *physics_world)
{
	this->ground_id = 0;

	//this->physics_body_def.position.Set(-10.0f, -10.0f);
	this->physics_body = physics_world->CreateBody(&(this->physics_body_def));
}