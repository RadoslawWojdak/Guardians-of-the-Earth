#include "water.h"

unsigned int cWater::water_id = 0;
b2BodyDef cWater::physics_body_def;
b2Body *cWater::physics_body = NULL;

void waterInit(b2World *physics_world)
{
	cWater water;
	water.resetMainParameters(physics_world);
};

cWater::cWater()
{
	;
}

cWater::cWater(sf::Texture &texture, sf::Vector2f pos)
{
	this->adjustGraphicsParameters(texture, pos);

	//BOX2D
	b2PolygonShape shape;
	const float32 x = pos.x * 0.02f;
	const float32 y = pos.y * 0.02f;

	shape.SetAsBox(0.02f * 32.0f / 2.0f, 0.02f * 32.0f / 2.0f, b2Vec2(x, y), 0.0f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_WATER);
	//fd.filter.maskBits = CATEGORY(CAT_TREASURE) | CATEGORY(CAT_NPC) | CATEGORY(CAT_TRAMPOLINE);
	fd.friction = 0.0f;
	this->physics_body->CreateFixture(&fd);

	this->water_id++;
}

void cWater::graphicsCustomize(sf::Vector2u map_size, bool *water_exists, sf::Vector2i grid_size)
{
	//Pozycja wody
	int x = getPosition().x / 32;
	int y = getPosition().y / 32 - 1;

	//Czy ponad wod¹ istnieje inna woda?
	bool exists = false;

	//Dostosowywanie grafiki wody do pozosta³ej wody
	if (y == 0 || water_exists[(y - 1) * grid_size.x + x])	//GÓRA
		exists = true;

	/*for (int i = 0; i < water.size(); i++)
	{
		//Pozycja aktualnie sprawdzanej wody
		int x2 = water[i].getPosition().x / 32;
		int y2 = water[i].getPosition().y / 32;

		if ((x == x2) && (y - 1 == y2))	//Czy ponad t¹ wod¹ znajduje siê inna woda?
		{
			exists = true;
			break;
		}
	}*/

	if (exists)
		this->setTexture(t_object[1]);	//Woda pod innymi wodami
	else
		this->setTexture(t_object[2]);	//Woda na samej górze
}

void cWater::resetMainParameters(b2World *physics_world)
{
	this->water_id = 0;

	this->physics_body = physics_world->CreateBody(&(this->physics_body_def));
}