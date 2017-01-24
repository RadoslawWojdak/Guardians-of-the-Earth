#include "ground.h"

unsigned int cGround::ground_id = 0;
b2BodyDef cGround::physics_body_def;
b2Body *cGround::physics_body = NULL;

void groundsInit(b2World &physics_world)
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
	
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_GROUND);
	fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE) | CATEGORY(CAT_CHARACTER) | CATEGORY(CAT_BULLET);
	fd.friction = 0.0f;
	this->physics_body->CreateFixture(&fd);

	this->ground_id++;
}

void cGround::graphicsCustomize(sf::Vector2u map_size, bool *ground_exists, sf::Vector2i grid_size)
{
	//Pozycja ziemii
	int x = getPosition().x / 32;
	int y = getPosition().y / 32 - 1;

	/*Bool exists[ - czy z tej strony bloku znajduje sie inny blok
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


	setTexture(t_ground[this->world_type][5]);		//Bez krawêdzi - domyœlny

	/*Legenda istnienia gruntów na danej pozycji
	- - Mog¹ byæ ale nie musz¹
	X - Nie mog¹ byæ
	1 - Musz¹ byæ
	*/

	/*ground 1
	-X-
	X X
	-X-*/
	if ((!exists[1]) && (!exists[3]) && (!exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][0]);
	/*ground 2
	-X-
	X 1
	-11*/
	else if ((!exists[1]) && (!exists[3]) && (exists[4]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][1]);
	/*ground 3
	-X-
	1 1
	111*/
	else if ((!exists[1]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][2]);
	/*ground 4
	-X-
	1 X
	11-*/
	else if ((!exists[1]) && (exists[3]) && (!exists[4]) && (exists[5]) && (exists[6]))
		setTexture(t_ground[this->world_type][3]);
	/*ground 5
	-11
	X 1
	-11*/
	else if ((exists[1]) && (exists[2]) && (!exists[3]) && (exists[4]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][4]);
	//Ground 6 pomijamy (bez krawêdzi) - je¿eli nie bêdzie pasowa³ ¿aden inny, to bêdzie znajdowa³ siê tamten
	/*ground 7
	11-
	1 X
	11-*/
	else if ((exists[0]) && (exists[1]) && (exists[3]) && (!exists[4]) && (exists[5]) && (exists[6]))
		setTexture(t_ground[this->world_type][6]);
	/*ground 8
	-11
	X 1
	-X-*/
	else if ((exists[1]) && (exists[2]) && (!exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][7]);
	/*ground 9
	111
	1 1
	-X-*/
	else if ((exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][8]);
	/*ground 10
	11-
	1 X
	-X-*/
	else if ((exists[0]) && (exists[1]) && (exists[3]) && (!exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][9]);
	/*ground 11
	111
	1 1
	11X*/
	else if ((exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][10]);
	/*ground 12
	111
	1 1
	X11*/
	else if ((exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][11]);
	/*ground 13
	11X
	1 1
	111*/
	else if ((exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][12]);
	/*ground 14
	X11
	1 1
	111*/
	else if ((!exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][13]);
	/*ground 15
	-X-
	X 1
	-X-*/
	else if ((!exists[1]) && (!exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][14]);
	/*ground 16
	-X-
	1 1
	-X-*/
	else if ((!exists[1]) && (exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][15]);
	/*ground 17
	-X-
	1 X
	-X-*/
	else if ((!exists[1]) && (exists[3]) && (!exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][16]);
	/*ground 18
	-X-
	X X
	-1-*/
	else if ((!exists[1]) && (!exists[3]) && (!exists[4]) && (exists[6]))
		setTexture(t_ground[this->world_type][17]);
	/*ground 19
	-1-
	X X
	-1-*/
	else if ((exists[1]) && (!exists[3]) && (!exists[4]) && (exists[6]))
		setTexture(t_ground[this->world_type][18]);
	/*ground 20
	-1-
	X X
	-X-*/
	else if ((exists[1]) && (!exists[3]) && (!exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][19]);
	/*ground 21
	-1X
	X 1
	-1X*/
	else if ((exists[1]) && (!exists[2]) && (!exists[3]) && (exists[4]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][20]);
	/*ground 22
	-X-
	1 1
	X1X*/
	else if ((!exists[1]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][21]);
	/*ground 23
	X1-
	1 X
	X1-*/
	else if ((!exists[0]) && (exists[1]) && (exists[3]) && (!exists[4]) && (!exists[5]) && (exists[6]))
		setTexture(t_ground[this->world_type][22]);
	/*ground 24
	X1X
	1 1
	-X-*/
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][23]);
	/*ground 25
	X1X
	1 1
	X1X*/
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][24]);
	/*ground 26
	-X-
	X 1
	-1-*/
	else if ((!exists[1]) && (!exists[3]) && (exists[4]) && (exists[6]))
		setTexture(t_ground[this->world_type][25]);
	/*ground 27
	-X-
	1 X
	-1-*/
	else if ((!exists[1]) && (exists[3]) && (!exists[4]) && (exists[6]))
		setTexture(t_ground[this->world_type][26]);
	/*ground 28
	-1-
	X 1
	-X-*/
	else if ((exists[1]) && (!exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][27]);
	/*ground 29
	-1-
	1 X
	-X-*/
	else if ((exists[1]) && (exists[3]) && (!exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][28]);
	/*ground 30
	-X-
	1 1
	11X*/
	else if ((!exists[1]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][29]);
	/*ground 31
	-X-
	1 1
	X11*/
	else if ((!exists[1]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][30]);
	/*ground 32
	-11
	X 1
	-1X*/
	else if ((exists[1]) && (exists[2]) && (!exists[3]) && (exists[4]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][31]);
	/*ground 33
	-1X
	X 1
	-11*/
	else if ((exists[1]) && (!exists[2]) && (!exists[3]) && (exists[4]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][32]);
	/*ground 34
	11-
	1 X
	X1-*/
	else if ((exists[0]) && (exists[1]) && (exists[3]) && (!exists[4]) && (!exists[5]) && (exists[6]))
		setTexture(t_ground[this->world_type][33]);
	/*ground 35
	X1-
	1 X
	11-*/
	else if ((!exists[0]) && (exists[1]) && (exists[3]) && (!exists[4]) && (exists[5]) && (exists[6]))
		setTexture(t_ground[this->world_type][34]);
	/*ground 36
	11X
	1 1
	-X-*/
	else if ((exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][35]);
	/*ground 37
	X11
	1 1
	-X-*/
	else if ((!exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[6]))
		setTexture(t_ground[this->world_type][36]);
	/*ground 38
	11X
	1 1
	11X*/
	else if ((exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][37]);
	/*ground 39
	111
	1 1
	X1X*/
	else if ((exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][38]);
	/*ground 40
	X11
	1 1
	X11*/
	else if ((!exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][39]);
	/*ground 41
	X1X
	1 1
	111*/
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][40]);
	/*ground 42
	11X
	1 1
	X11*/
	else if ((exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][41]);
	/*ground 43
	X11
	1 1
	11X*/
	else if ((!exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][42]);
	/*ground 44
	11X
	1 1
	X1X*/
	else if ((exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][43]);
	/*ground 45
	X11
	1 1
	X1X*/
	else if ((!exists[0]) && (exists[1]) && (exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][44]);
	/*ground 46
	X1X
	1 1
	X11*/
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (!exists[5]) && (exists[6]) && (exists[7]))
		setTexture(t_ground[this->world_type][45]);
	/*ground 47
	X1X
	1 1
	11X*/
	else if ((!exists[0]) && (exists[1]) && (!exists[2]) && (exists[3]) && (exists[4]) && (exists[5]) && (exists[6]) && (!exists[7]))
		setTexture(t_ground[this->world_type][46]);
}

void cGround::resetMainParameters(b2World &physics_world)
{
	this->ground_id = 0;
	this->physics_body = physics_world.CreateBody(&(this->physics_body_def));
}

b2Body *cGround::getBody()
{
	return this->physics_body;
}